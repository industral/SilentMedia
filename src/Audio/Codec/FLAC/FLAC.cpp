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
      // FLAC: Public methods
      // --------------------------------------------------------------------

      FLAC::FLAC() {
        // create instance for AudioProxy
        this -> audioProxy = new AudioProxy();
      }

      FLAC::~FLAC() {
        // release resources
        delete this -> audioProxy;
        this -> audioProxy = NULL;
      }

      bool FLAC::open(const string &fileId) {
        // get fileName
        string fileName = this -> audioProxy -> getFileNameByFileId(fileId);

        if (!boost::filesystem::exists(fileName)) {
          return false;
        }

        this -> flacDecoderMap[fileId] = new FLACDecoder();

        this -> templateInitFile(fileId, fileName);

        this -> flacDecoderMap[fileId] -> process_single();

        this -> parseMetaData(fileId);

        return true;
      }

      int FLAC::play(const string &fileId, bool resume) {
        // set params if it in first time
        if (!resume) {
          // now set parameters to sound system
          this -> audioProxy -> setSoundSystemParams(fileId);
        }

        this -> flacDecoderMap[fileId] -> setFileId(fileId);

        cout << "FLAC in play()" << endl;

        while (1) {
          if (this -> stopMap[fileId]) {
            cout << "EXIT" << endl;
            this -> close(fileId);
            return 0;
          } else {
            this -> flacDecoderMap[fileId] -> process_single();
            if (this -> flacDecoderMap[fileId] -> get_state()
                == FLAC__STREAM_DECODER_END_OF_STREAM) {
              this -> close(fileId);
              cout << "End of stream." << endl;
              return 0;
            }
          }
        }
        return 0;
      }

      void FLAC::stop(const string &fileId) {
        this -> stopMap[fileId] = true;
      }

      void FLAC::close(const string &fileId) {
        if (this -> flacDecoderMap[fileId] != NULL) {
          this -> flacDecoderMap[fileId] -> finish();
          this -> flacDecoderMap[fileId] -> flush();

          delete this -> flacDecoderMap[fileId];
          this -> flacDecoderMap[fileId] = NULL;

          // clean maps
          this -> flacDecoderMap.erase(fileId);
        }

        delete this -> streamInfoMap[fileId];
        this -> streamInfoMap[fileId] = NULL;

        delete this -> iteratorMap[fileId];
        this -> iteratorMap[fileId] = NULL;
      }

      float FLAC::getSeek(const string &fileId) {
        double currentSamples = 0.0;
        FLAC__uint64 totalSamples = 0;

        // check if object is not deleted, as song stop - it delete
        if (this -> flacDecoderMap[fileId] != NULL) {
          totalSamples = this -> flacDecoderMap[fileId] -> get_total_samples();

          currentSamples = this -> audioProxy -> getCurrentSamples(fileId);
        }

        if (currentSamples == 0) {
          return 0;
        } else {
          return (100 / (totalSamples / currentSamples));
        }
      }

      void FLAC::setSeek(const string &fileId, const double &seekVal) {
        FLAC__uint64 totalSamples =
            this -> flacDecoderMap[fileId] -> get_total_samples();

        FLAC__uint64 sample_pos = ((totalSamples * (seekVal / 100)));

        if (!this -> flacDecoderMap[fileId] -> seek_absolute(sample_pos)) {
          cerr << "Error in seek_absolute()" << endl;
          this -> flacDecoderMap[fileId] -> flush();
        }

        this -> audioProxy -> setCurrentSamples(fileId, sample_pos);
      }

      // --------------------------------------------------------------------
      // FLAC: Private methods
      // --------------------------------------------------------------------

      bool FLAC::templateInitFile(const string &fileId, const string &fileName) {

        FLAC__StreamDecoderInitStatus retCode =
            this -> flacDecoderMap[fileId] -> init(fileName.c_str());

        switch (retCode) {
        case FLAC__STREAM_DECODER_INIT_STATUS_OK:
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER:
          cerr << "Error in init(): "
              << "The library was not compiled with support for the given container format"
              << endl;
          return false;
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_INVALID_CALLBACKS:
          cerr << "Error in init(): " << "A required callback was not supplied"
              << endl;
          return false;
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_MEMORY_ALLOCATION_ERROR:
          cerr << "Error in init(): " << "An error occurred allocating memory"
              << endl;
          return false;
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_ERROR_OPENING_FILE:
          cerr << "Error in init(): "
              << "fopen() failed in FLAC__stream_decoder_init_file() or FLAC__stream_decoder_init_ogg_file()"
              << endl;
          return false;
          break;
        case FLAC__STREAM_DECODER_INIT_STATUS_ALREADY_INITIALIZED:
          cerr << "Error in init(): "
              << "FLAC__stream_decoder_init_*() was called when the decoder was already initialized, usually because FLAC__stream_decoder_finish() was not called."
              << endl;
          return false;
          break;
        }
        return true;
      }

      void FLAC::parseMetaData(const string &fileId) {
        // get file name
        string fileName = this -> audioProxy -> getFileNameByFileId(fileId);

        this -> iteratorMap[fileId] = new ::FLAC::Metadata::SimpleIterator;
        this -> iteratorMap[fileId] -> init(fileName.c_str(), true, true);

        this -> streamInfoMap[fileId]
            = new ::FLAC::Metadata::StreamInfo::StreamInfo;

        ::FLAC::Metadata::get_streaminfo(fileName.c_str(),
            *this -> streamInfoMap[fileId]);

        long fileSize = boost::filesystem::file_size(fileName);

        double totalTime = 0.0;
        // check to avoid float point exception
        if (this -> streamInfoMap[fileId] -> get_sample_rate() > 0) {
          totalTime = (this -> flacDecoderMap[fileId] -> get_total_samples()
              / this -> streamInfoMap[fileId] -> get_sample_rate());
        }

        int size = 0; // переменная для инкремента размера всех найденых блоков
        while (!this -> iteratorMap[fileId] -> is_last()) {
          switch (this -> iteratorMap[fileId] -> get_block_type()) {
          case FLAC__METADATA_TYPE_STREAMINFO:
            //                   cout << "FLAC__METADATA_TYPE_STREAMINFO" << endl;
            break;
          case FLAC__METADATA_TYPE_PADDING:
            //                   cout << "FLAC__METADATA_TYPE_PADDING" << endl;
            break;
          case FLAC__METADATA_TYPE_APPLICATION:
            //                   cout << "FLAC__METADATA_TYPE_APPLICATION" << endl;
            break;
          case FLAC__METADATA_TYPE_SEEKTABLE:
            //                   cout << "FLAC__METADATA_TYPE_SEEKTABLE" << endl;
            break;
          case FLAC__METADATA_TYPE_VORBIS_COMMENT:
            //                   cout << "FLAC__METADATA_TYPE_VORBIS_COMMENT" << endl;
            this -> readVorbisComment(fileId);
            break;
          case FLAC__METADATA_TYPE_CUESHEET:
            //                   cout << "FLAC__METADATA_TYPE_CUESHEET" << endl;
            break;
          case FLAC__METADATA_TYPE_PICTURE:
            //                   cout << "FLAC__METADATA_TYPE_PICTURE" << endl;
            //            this -> getPicture();
            break;
          case FLAC__METADATA_TYPE_UNDEFINED:
            //                   cout << "FLAC__METADATA_TYPE_UNDEFINED" << endl;
            break;
          }
          this -> iteratorMap[fileId] -> next(); // движемся далее
          size += this -> iteratorMap[fileId] -> get_block_length();
        }

        // get size of useful data
        int streamSize = fileSize - size;
        // get the average bitrate
        double avrBitrate = streamSize / totalTime * 8;

        this -> audioProxy -> setAudioParams(fileId, fileName, fileSize,
            totalTime, this -> streamInfoMap[fileId] -> get_channels(),
            this -> streamInfoMap[fileId] -> get_sample_rate(), avrBitrate,
            this -> streamInfoMap[fileId] -> get_bits_per_sample());
      }

      // --------------------------------------------------------------------
      // FLACDecoder: Public methods
      // --------------------------------------------------------------------

      FLACDecoder::FLACDecoder() {
        this -> flacObj = new FLAC();
      }

      FLAC__StreamDecoderWriteStatus FLACDecoder::write_callback(
          const FLAC__Frame * frame, const FLAC__int32 * const buf[]) {

        const int decoded_size = frame -> header.blocksize
            * frame -> header.channels * (frame -> header.bits_per_sample / 8); // 16384, 18432

        int samples = frame -> header.blocksize;
        int sample = 0;
        int i = 0;
        unsigned int channel = 0; // use unsigned to avoid warning from compile

        int16_t outbuf[FLAC__MAX_BLOCK_SIZE * FLAC__MAX_CHANNELS
            * sizeof(int32_t)]; // 65535 * 8 * 4

        //        cout << "A: " << decoded_size << " B: " << samples << endl;

        for (sample = i = 0; sample < samples; sample++) {
          for (channel = 0; channel < frame -> header.channels; channel++, i++) {
            outbuf[i] = buf[channel][sample];
          }
        }

        //        cout << "Size: " << sizeof(outbuf) << endl;

        double totalSamples =
            this -> flacObj -> audioProxy -> getCurrentSamples(this -> fileId);

        this -> flacObj -> audioProxy -> setCurrentSamples(this -> fileId,
            totalSamples + sample);

        this -> flacObj -> audioProxy -> write(outbuf, decoded_size);

        return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
      }

      void FLACDecoder::metadata_callback(const FLAC__StreamMetadata * block) {
      }

      /*
       * Callback error catch function.
       */
      void FLACDecoder::error_callback(::FLAC__StreamDecoderErrorStatus status) {
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

      void FLACDecoder::setFileId(const string &fileId) {
        this -> fileId = fileId;
      }

      void FLAC::readVorbisComment(const string &fileId) {
        // get file name
        string fileName = this -> audioProxy -> getFileNameByFileId(fileId);

        map < string, string > vorbisComments;

        ::FLAC::Metadata::VorbisComment * tags =
            new ::FLAC::Metadata::VorbisComment;
        ::FLAC::Metadata::get_tags(fileName.c_str(), tags);

        // using unsigned, avoid warning
        for (uint i = 0; i < tags -> get_num_comments(); ++i) {
          ::FLAC::Metadata::VorbisComment::Entry entry = tags -> get_comment(i);

          string key = entry.get_field_name();
          transform(key.begin(), key.end(), key.begin(), ::toupper);

          vorbisComments[key] = entry.get_field_value();
        }

        this -> audioProxy -> setVorbisComment(fileId, vorbisComments);

        delete tags;
        tags = NULL;
      }

    //      void FLAC::getPicture() {
    //        //             this -> picture = new ::FLAC::Metadata::Picture();
    //        ::FLAC::Metadata::Picture picture;
    //        multimap < FLAC__StreamMetadata_Picture_Type, string >
    //            picData;
    //
    //        //          Proxy < ::FLAC::Metadata::Picture > picture;
    //
    //        //            int count = 0;
    //        if (::FLAC::Metadata::get_picture(this-> fileName.c_str(), picture,
    //			static_cast<::FLAC__StreamMetadata_Picture_Type> (-1), NULL, NULL,
    //			-1, -1, -1, -1) == true) {
    //
    //          this -> spdata.clear();
    //          //                  picture -> clear();
    //          //                  cout << "MYYYYY COUNT: " << count << endl;
    //
    //          const FLAC__byte * pdata = picture.get_data();
    //
    //          for (int i = 0; i < picture.get_length(); ++i) {
    //            spdata += pdata[i];
    //          }
    //          cout << "PICTURE!!!" << endl;
    //          cout << "PICTURE: " << picture.get_data_length() << endl;
    //          picData.insert(pair<::FLAC__StreamMetadata_Picture_Type,
    //              string>(picture.get_type(), spdata));
    //          cout << "FLAC COUNT: " << picData.size() << endl;
    //        }
    //        //               this -> ddata.setPicture ( picData );
    //        //               delete picture; picture = 0;
    //      }


    //      void FLAC::setPicture(string coverData) {
    //        ::FLAC::Metadata::Picture * picture = new ::FLAC::Metadata::Picture;
    //        // конвертируем данные в coverData нужные set_data() в const FLAC__byte *
    //        const FLAC__byte * data = (const FLAC__byte *) coverData.c_str();
    //
    //        if (picture -> set_data(data, coverData.size()) == false) {
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
    //        picture -> set_width(imagick -> size().width());
    //        picture -> set_height(imagick -> size().height());
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

    }
  }
}

