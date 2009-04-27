/*******************************************************************************
 * Copyright (c) 2009, Alex Ivasyuv                                            *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *                                                                             *
 * 1. Redistributions of source code must retain the above copyright           *
 *    notice, this list of conditions and the following disclaimer.            *
 * 2. Redistributions in binary form must reproduce the above copyright        *
 *    notice, this list of conditions and the following disclaimer in the      *
 *    documentation and/or other materials provided with the distribution.     *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY Alex Ivasyuv ''AS IS'' AND ANY                 *
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE      *
 * DISCLAIMED. IN NO EVENT SHALL Alex Ivasyuv BE LIABLE FOR ANY DIRECT,        *
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES          *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;*
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF    *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           *
 ******************************************************************************/

#include "FLAC.hpp"

namespace SilentMedia {
  namespace Audio {
    namespace Codec {

      // --------------------------------------------------------------------
      // Functions
      // --------------------------------------------------------------------

      FLAC__StreamDecoderWriteStatus write_callback(
          const FLAC__StreamDecoder * dec, const FLAC__Frame * frame,
          const FLAC__int32 * const buf[], void * client_data) {
        //          cout << "write_callback" << endl;

        FLAC * flacObj = static_cast < FLAC * > (client_data);
        int decoded_size = frame -> header.blocksize * frame -> header.channels
            * (frame -> header.bits_per_sample / 8); // 16384, 18432

        int samples = frame -> header.blocksize;
        int sample = 0;
        int i = 0;
        short int channel = 0;

        int16_t outbuf[FLAC__MAX_BLOCK_SIZE * FLAC__MAX_CHANNELS
            * sizeof(int32_t)]; // 65535 * 8 * 4

        for (sample = i = 0; sample < samples; sample++) {
          for (channel = 0; channel < frame -> header.channels; channel++, i++) {
            outbuf[i] = buf[channel][sample];
          }
          //          flacObj -> setCurrSample(flacObj -> getCurrSample() + 1);
        }
        //        write(flacObj -> getdspDev(), outbuf, decoded_size);
        return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
      }

      void metadata_callback(const FLAC__StreamDecoder * dec,
          const FLAC__StreamMetadata * block, void * client_data) {
      }

      // callback-ф-ция для перехвата ошибок
      void error_callback(const ::FLAC__StreamDecoder *decoder,
          ::FLAC__StreamDecoderErrorStatus status, void *client_data) {
        if (status == FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC) {
          cerr
              << "Error: error_callback(): An error in the stream caused the decoder to lose synchronization"
              << endl;
        } else if (status == FLAC__STREAM_DECODER_ERROR_STATUS_BAD_HEADER) {
          cerr
              << "Error: error_callback(): The decoder encountered a corrupted frame header"
              << endl;
        } else if (status
            == FLAC__STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH) {
          cerr
              << "Error: error_callback(): The frame's data did not match the CRC in the footer"
              << endl;
        } else if (status
            == FLAC__STREAM_DECODER_ERROR_STATUS_UNPARSEABLE_STREAM) {
          cerr
              << "Error: error_callback(): The decoder encountered reserved fields in use in the stream"
              << endl;
        }
      }

      bool FLAC::templateInitFile(FLAC__StreamDecoder * dec) {

        FLAC__StreamDecoderInitStatus retCode = FLAC__stream_decoder_init_file(
            dec, this -> fileName.c_str(), write_callback, metadata_callback,
            error_callback, this);

        switch (retCode) {
        case FLAC__STREAM_DECODER_INIT_STATUS_OK:
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER:
          cerr << "Error in FLAC__stream_decoder_init_file(): "
              << "The library was not compiled with support for the given container format"
              << endl;
          return false;
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_INVALID_CALLBACKS:
          cerr << "Error in FLAC__stream_decoder_init_file(): "
              << "A required callback was not supplied" << endl;
          return false;
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_MEMORY_ALLOCATION_ERROR:
          cerr << "Error in FLAC__stream_decoder_init_file(): "
              << "An error occurred allocating memory" << endl;
          return false;
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_ERROR_OPENING_FILE:
          cerr << "Error in FLAC__stream_decoder_init_file(): "
              << "fopen() failed in FLAC__stream_decoder_init_file() or FLAC__stream_decoder_init_ogg_file()"
              << endl;
          return false;
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_ALREADY_INITIALIZED:
          cerr << "Error in FLAC__stream_decoder_init_file(): "
              << "FLAC__stream_decoder_init_*() was called when the decoder was already initialized, usually because FLAC__stream_decoder_finish() was not called."
              << endl;
          return false;
          break;
        }
        return true;
      }

      // --------------------------------------------------------------------
      // Public methods
      // --------------------------------------------------------------------

      FLAC::FLAC(void) {
        // create instance for AudioProxy
        this -> audioProxy = new AudioProxy();
      }

      FLAC::~FLAC() {
        // release resources
        delete this -> audioProxy;
        this -> audioProxy = NULL;
      }

      bool FLAC::open(const string &fileId) {
        //        this -> dspDev = this -> ddata -> getDSPDev();
        //        cout << "FLAC in init()" << " : " << getV() << endl;
        //        this -> fileName = inputFile;

        // get fileName
        this -> fileNameMap[fileId]
            = this -> audioProxy -> getFileNameByFileId(fileId);

        this -> decoder = FLAC__stream_decoder_new();
        this -> templateInitFile(this -> decoder);

        // обязательно для заполнения стуктуры
        FLAC__stream_decoder_process_single(this -> decoder);

        //        this -> parseMetaData();

        return true;
      }

      void FLAC::play(const string &fileId, bool resume) {
        if (!this -> seekCheck) { // если мы делаем seek то выделать заново память не нужно
          this -> pdecoder = FLAC__stream_decoder_new();
        }
        if (this -> pdecoder == NULL) {
          cout << "Error to FLAC__stream_decoder_new() " << endl;
        }
        if (!this -> playCheck) {
          //          this -> templateInitFile(this -> pdecoder);
        }
        this -> playCheck = true;
        // обязательно для заполнения стуктуры
        FLAC__stream_decoder_process_single(this -> pdecoder);
        this -> totalSamples = FLAC__stream_decoder_get_total_samples(
            this -> pdecoder);

        cout << "FLAC in play()" << endl;

        //        this -> ddata -> setParam();
        while (FLAC__stream_decoder_process_until_end_of_stream(
            this -> pdecoder)) {
          if (FLAC__stream_decoder_get_state(this -> pdecoder)
              == FLAC__STREAM_DECODER_END_OF_STREAM) {
            cout << "End of stream." << endl;
          }
          break;
        }
      }

      float FLAC::getSeek(const string &fileId) {
        if (!this -> curr_sample) {
          return 0;
        } else {
          return (100 / (this -> totalSamples / this -> curr_sample));
        }
      }

      void FLAC::setSeek(const string &fileId, const double &seekVal) {
        this -> seekCheck = true;
        FLAC__uint64 sample_pos = (((this -> totalSamples) * (seekVal / 100)));
        if (FLAC__stream_decoder_seek_absolute(this -> pdecoder, sample_pos)
            != true) {
          cout << "Error in FLAC__stream_decoder_seek_absolute()" << endl;
        }
        //        this -> setCurrSample(sample_pos);
      }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      //      void FLAC::readVorbisComment() {
      //        this -> vorbisComm . clear();
      //
      //        ::FLAC::Metadata::VorbisComment * tags =
      //            new ::FLAC::Metadata::VorbisComment;
      //        ::FLAC::Metadata::get_tags(this-> fileName . c_str(), tags);
      //
      //        for (int i = 0; i != tags -> get_num_comments(); ++i) {
      //          ::FLAC::Metadata::VorbisComment::Entry entry = tags -> get_comment(i);
      //          vorbisComm[::Utils::Func::toUpper(entry . get_field_name())]
      //              = entry . get_field_value();
      //        }
      //
      //        this -> ddata -> setVorbisComment(this -> vorbisComm);
      //        delete tags;
      //      }

      //      void FLAC::getPicture() {
      //        //             this -> picture = new ::FLAC::Metadata::Picture();
      //        ::FLAC::Metadata::Picture picture;
      //        multimap < FLAC__StreamMetadata_Picture_Type, string >
      //            picData;
      //
      //        //          Proxy < ::FLAC::Metadata::Picture > picture;
      //
      //        //           short int count = 0;
      //        if (::FLAC::Metadata::get_picture(this-> fileName . c_str(), picture,
      //			static_cast<::FLAC__StreamMetadata_Picture_Type> (-1), NULL, NULL,
      //			-1, -1, -1, -1) == true) {
      //
      //          this -> spdata . clear();
      //          //                  picture -> clear();
      //          //                  cout << "MYYYYY COUNT: " << count << endl;
      //
      //          const FLAC__byte * pdata = picture . get_data();
      //
      //          for (int i = 0; i < picture . get_length(); ++i) {
      //            spdata += pdata[i];
      //          }
      //          cout << "PICTURE!!!" << endl;
      //          cout << "PICTURE: " << picture . get_data_length() << endl;
      //          picData . insert(pair<::FLAC__StreamMetadata_Picture_Type,
      //              string>(picture . get_type(), spdata));
      //          cout << "FLAC COUNT: " << picData.size() << endl;
      //        }
      //        //               this -> ddata . setPicture ( picData );
      //        //               delete picture; picture = 0;
      //      }

      //      void FLAC::setDSPParam() {
      //        this -> ddata -> setAudioParams(this -> fileSize, this -> totalTime,
      //            this -> streamInfo -> get_channels(),
      //            this -> streamInfo -> get_sample_rate(), this -> avrBitrate,
      //            this -> streamInfo -> get_bits_per_sample());
      //      }

      //      void FLAC::setPicture(string coverData) {
      //        ::FLAC::Metadata::Picture * picture = new ::FLAC::Metadata::Picture;
      //        // конвертируем данные в coverData нужные set_data() в const FLAC__byte *
      //        const FLAC__byte * data = (const FLAC__byte *) coverData.c_str();
      //
      //        if (picture -> set_data(data, coverData . size()) == false) {
      //          cout << "Error to set picture!" << endl;
      //        }
      //
      //        // инициализируем итератор в режиме чтение-запись
      //        iterator -> init(this -> fileName.c_str(), false, true);
      //
      //        // Используем ImageMagik дляполучения параметров рисунка ( высота, ширина, глубина, тип )
      //        Magick::Image * imagick = new Magick::Image;
      //        Magick::Blob * blob = new Magick::Blob(coverData.c_str(),
      //            coverData.size());
      //        imagick -> read(*blob);
      //
      //        // составляем mime-тип ( напр. "image/png" )
      //        string mime = "image/" + Utils::Func::toLower(string(
      //            imagick -> magick()));
      //
      //        // записываем эти значения в обект picture
      //        picture -> set_mime_type(mime.c_str());
      //        picture -> set_width(imagick -> size() . width());
      //        picture -> set_height(imagick -> size() . height());
      //        picture -> set_depth(imagick -> depth());
      //
      //        // запись в файл
      //        iterator -> insert_block_after(picture);
      //
      //        // освобождаем память
      //        delete imagick;
      //        delete blob;
      //        delete picture;
      //        delete iterator;
      //      }

      void FLAC::close(const string &fileId) {
        //        FLAC__stream_decoder_finish(this -> decoder);
        //        FLAC__stream_decoder_flush(this -> decoder);
      }
    //
    //      void FLAC::flush() {
    //        cout << "FLAC in flush()" << endl;
    //        /*
    //         Ставим стражей в исходное положение.
    //         Возможно следовало как то бы пересоздавать обяек?
    //         */
    //        this -> playCheck = false;
    //        this -> seekCheck = false;
    //
    //        this -> setCurrSample(0);
    //        //          this -> setSeekPos ( 0 );
    //
    //        //          if ( ( this -> pdecoder ) != NULL ) {
    //        //             FLAC__stream_decoder_finish ( this -> pdecoder );
    //        //             FLAC__stream_decoder_flush ( this -> pdecoder );
    //        //          }
    //        //          if ( ( this -> decoder ) != NULL ) {
    //        //             FLAC__stream_decoder_finish ( this -> decoder );
    //        //             FLAC__stream_decoder_finish ( this -> decoder );
    //        //          }
    //        // FLAC__stream_decoder_flush ( this -> dec );
    //
    //        //          FLAC__stream_decoder_delete ( decoder );
    //        // delete streamInfo;
    //        //          delete decoder;
    //
    //      }

    //      void FLAC::finish() {
    //        //          FLAC__stream_decoder_finish ( this -> dec );
    //        //          FLAC__stream_decoder_flush ( this -> dec );
    //      }

    //      void FLAC::parseMetaData() {
    //        iterator -> init(this -> fileName.c_str(), false, true);
    //
    //        ::FLAC::Metadata::get_streaminfo(this -> fileName.c_str(),
    //            *this -> streamInfo);
    //
    //        this -> fileSize = Utils::Func::getFileSize(this -> fileName);
    //        // применяем сдесь ф-цию FLAC__stream_decoder_get_total_samples() !
    //        this -> totalTime = (FLAC__stream_decoder_get_total_samples(
    //            this -> decoder) / this -> streamInfo -> get_sample_rate());
    //
    //        int size = 0; // переменная для инкремента размера всех найденых блоков
    //        while (!iterator -> is_last()) {
    //          switch (iterator -> get_block_type()) {
    //          case FLAC__METADATA_TYPE_STREAMINFO:
    //            //                   cout << "FLAC__METADATA_TYPE_STREAMINFO" << endl;
    //            break;
    //          case FLAC__METADATA_TYPE_PADDING:
    //            //                   cout << "FLAC__METADATA_TYPE_PADDING" << endl;
    //            break;
    //          case FLAC__METADATA_TYPE_APPLICATION:
    //            //                   cout << "FLAC__METADATA_TYPE_APPLICATION" << endl;
    //            break;
    //          case FLAC__METADATA_TYPE_SEEKTABLE:
    //            //                   cout << "FLAC__METADATA_TYPE_SEEKTABLE" << endl;
    //            break;
    //          case FLAC__METADATA_TYPE_VORBIS_COMMENT:
    //            //                   cout << "FLAC__METADATA_TYPE_VORBIS_COMMENT" << endl;
    //            this -> readVorbisComment();
    //            break;
    //          case FLAC__METADATA_TYPE_CUESHEET:
    //            //                   cout << "FLAC__METADATA_TYPE_CUESHEET" << endl;
    //            break;
    //          case FLAC__METADATA_TYPE_PICTURE:
    //            //                   cout << "FLAC__METADATA_TYPE_PICTURE" << endl;
    //            this -> getPicture();
    //            break;
    //          case FLAC__METADATA_TYPE_UNDEFINED:
    //            //                   cout << "FLAC__METADATA_TYPE_UNDEFINED" << endl;
    //            break;
    //          }
    //          iterator -> next(); // движемся далее
    //          size += iterator -> get_block_length();
    //        }
    //        // получаем размер полезных аудио-данных
    //        this -> streamSize = this -> fileSize - size;
    //        // получаем средний битрейт
    //        this -> avrBitrate = this -> streamSize / this -> totalTime * 8;
    //        this -> setDSPParam();
    //      }
    }
  }
}

