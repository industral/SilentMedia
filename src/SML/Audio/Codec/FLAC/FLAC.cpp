/***************************************************************************
*   Copyright (C) 2008 by Alex J. Ivasyuv                                 *
*   alex@siegerstein.org.ua                                               *
*                                                                         *
*   This program is free software: you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation, either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
***************************************************************************/

#include "FLAC.hpp"
#include <SML/Audio/Codec/DecodedData.hpp>

SilentMedia::Codec::FLAC::FLAC ( void ) :
      decoder ( NULL ), pdecoder ( NULL ),
               iterator ( new ::FLAC::Metadata::SimpleIterator ),
                playCheck ( false ), seekCheck ( false ), fileSize ( -1 ), streamSize ( -1 ), avrBitrate ( -1 ),
            totalSamples ( 0 ), totalTime ( -1 ), curr_sample ( 0 ), position ( new FLAC__uint64 ),
            streamInfo ( new ::FLAC::Metadata::StreamInfo::StreamInfo )
{
   this -> ddata = DecodedData::Instance();
}

            SilentMedia::Codec::FLAC::~FLAC ( void ) {
//    delete dsp;
}

FLAC__StreamDecoderWriteStatus write_callback ( const FLAC__StreamDecoder * dec, const FLAC__Frame * frame,
      const FLAC__int32 * const buf[], void * client_data ) {
//          std::cout << "write_callback" << std::endl;

         SilentMedia::Codec::FLAC * flacObj = static_cast < SilentMedia::Codec::FLAC * > ( client_data );
         int decoded_size = frame -> header.blocksize * frame -> header.channels * ( frame -> header.bits_per_sample / 8 ); // 16384, 18432

         unsigned int samples = frame -> header.blocksize;
         unsigned int sample = 0;
         unsigned int i = 0;
         unsigned short int channel = 0;

         int16_t outbuf [ FLAC__MAX_BLOCK_SIZE * FLAC__MAX_CHANNELS * sizeof ( int32_t ) ]; // 65535 * 8 * 4

         for ( sample = i = 0; sample < samples; sample++ ) {
            for ( channel = 0; channel < frame -> header.channels; channel++, i++ ) {
               outbuf [ i ] = buf [ channel ] [ sample ];
            }
            flacObj -> setCurrSample ( flacObj -> getCurrSample() + 1 );
         }
         write ( flacObj -> getdspDev(), outbuf, decoded_size );
         return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
      }

      void metadata_callback(const FLAC__StreamDecoder * dec, const FLAC__StreamMetadata * block, void * client_data ) { }

// callback-ф-ция для перехвата ошибок
      void error_callback ( const ::FLAC__StreamDecoder *decoder, ::FLAC__StreamDecoderErrorStatus status, void *client_data ) {
         if ( status == FLAC__STREAM_DECODER_ERROR_STATUS_LOST_SYNC ) {
            std::cout << "Error: error_callback(): An error in the stream caused the decoder to lose synchronization" << std::endl;
         } else if ( status == FLAC__STREAM_DECODER_ERROR_STATUS_BAD_HEADER ) {
            std::cout << "Error: error_callback(): The decoder encountered a corrupted frame header" << std::endl;
         } else if ( status == FLAC__STREAM_DECODER_ERROR_STATUS_FRAME_CRC_MISMATCH ) {
            std::cout << "Error: error_callback(): The frame's data did not match the CRC in the footer" << std::endl;
         } else if ( status == FLAC__STREAM_DECODER_ERROR_STATUS_UNPARSEABLE_STREAM ) {
            std::cout << "Error: error_callback(): The decoder encountered reserved fields in use in the stream" << std::endl;
         }
      }

      bool SilentMedia::Codec::FLAC::templateInitFile ( FLAC__StreamDecoder * dec ) {
         
         FLAC__StreamDecoderInitStatus retCode = FLAC__stream_decoder_init_file ( dec, this -> fileName.c_str(), write_callback, metadata_callback, error_callback, this );

         switch ( retCode ) {
            case FLAC__STREAM_DECODER_INIT_STATUS_OK:
               break;
            case FLAC__STREAM_DECODER_INIT_STATUS_UNSUPPORTED_CONTAINER:
               std::cout << "Error in FLAC__stream_decoder_init_file(): " << "The library was not compiled with support for the given container format" << std::endl;
               return false;
               break;
            case FLAC__STREAM_DECODER_INIT_STATUS_INVALID_CALLBACKS:
               std::cout << "Error in FLAC__stream_decoder_init_file(): " << "A required callback was not supplied" << std::endl;
               return false;
               break;
            case FLAC__STREAM_DECODER_INIT_STATUS_MEMORY_ALLOCATION_ERROR:
               std::cout << "Error in FLAC__stream_decoder_init_file(): " << "An error occurred allocating memory" << std::endl;
               return false;
               break;
            case FLAC__STREAM_DECODER_INIT_STATUS_ERROR_OPENING_FILE:
               std::cout << "Error in FLAC__stream_decoder_init_file(): " << "fopen() failed in FLAC__stream_decoder_init_file() or FLAC__stream_decoder_init_ogg_file()" << std::endl;
               return false;
               break;
            case FLAC__STREAM_DECODER_INIT_STATUS_ALREADY_INITIALIZED:
               std::cout << "Error in FLAC__stream_decoder_init_file(): " << "FLAC__stream_decoder_init_*() was called when the decoder was already initialized, usually because FLAC__stream_decoder_finish() was not called." << std::endl;
               return false;
               break;
         }
         return true;
      }
      
      bool SilentMedia::Codec::FLAC::init ( std::string inputFile ) {
         this -> dspDev = this -> ddata -> getDSPDev();
         std::cout << "FLAC in init()" << " : " << getV() << std::endl;
         this -> fileName = inputFile;

         this -> decoder = FLAC__stream_decoder_new();
         this -> templateInitFile ( this -> decoder );
              
         // обязательно для заполнения стуктуры
         FLAC__stream_decoder_process_single ( this -> decoder );

         this -> parseMetaData();

         return true;
      }
      
      void SilentMedia::Codec::FLAC::play ( void ) {
         if ( !this -> seekCheck ) { // если мы делаем seek то выделать заново память не нужно
            this -> pdecoder = FLAC__stream_decoder_new();
         }
         if ( this -> pdecoder == NULL ) {
            std::cout << "Error to FLAC__stream_decoder_new() " << std::endl;
         }
         if ( !this -> playCheck ) {
            this -> templateInitFile ( this -> pdecoder );
         }
         this -> playCheck = true;
         // обязательно для заполнения стуктуры
         FLAC__stream_decoder_process_single ( this -> pdecoder );
         this -> totalSamples = FLAC__stream_decoder_get_total_samples ( this -> pdecoder );

         std::cout << "FLAC in play()" << std::endl;
         
         this -> ddata -> setParam();
         while (FLAC__stream_decoder_process_until_end_of_stream ( this -> pdecoder ) ) {
            if ( FLAC__stream_decoder_get_state ( this -> pdecoder ) == FLAC__STREAM_DECODER_END_OF_STREAM ) {
               std::cout << "End of stream." << std::endl;
            }
            break;
         }
      }
      
      unsigned long int SilentMedia::Codec::FLAC::getCurrSeekPos ( void ) const {
         if ( !this -> curr_sample ) {
            return 0;
         } else {
            return ( 100 / ( this -> totalSamples / this -> curr_sample ) );
         }
      }

      void SilentMedia::Codec::FLAC::setSeekPos ( double val ) {
         this -> seekCheck = true;
         FLAC__uint64 sample_pos = ( ( ( this -> totalSamples ) * ( val / 100 ) ) );
         if ( FLAC__stream_decoder_seek_absolute ( this -> pdecoder,  sample_pos ) != true ) {
            std::cout << "Error in FLAC__stream_decoder_seek_absolute()" << std::endl;
         }
         this -> setCurrSample ( sample_pos );
      }

      void SilentMedia::Codec::FLAC::readVorbisComment ( void ) {
         this -> vorbisComm . clear();

            ::FLAC::Metadata::VorbisComment * tags = new ::FLAC::Metadata::VorbisComment;
            ::FLAC::Metadata::get_tags ( this-> fileName . c_str(), tags );

         for ( unsigned int i = 0; i != tags -> get_num_comments(); ++i ) {
               ::FLAC::Metadata::VorbisComment::Entry entry = tags -> get_comment ( i );
            vorbisComm [ ::Utils::Func::toUpper ( entry . get_field_name() ) ] = entry . get_field_value();
         }

         this -> ddata -> setVorbisComment ( this -> vorbisComm );
         delete tags;
      }

      void SilentMedia::Codec::FLAC::getPicture ( void ) {
//             this -> picture = new ::FLAC::Metadata::Picture();
            ::FLAC::Metadata::Picture picture;
         std::multimap < FLAC__StreamMetadata_Picture_Type, std::string > picData;

//          Proxy < ::FLAC::Metadata::Picture > picture;
         
//          unsigned short int count = 0;
         if ( ::FLAC::Metadata::get_picture ( this-> fileName . c_str(), picture, static_cast < ::FLAC__StreamMetadata_Picture_Type > ( -1 ), NULL,
              NULL, -1, -1, -1, -1 ) == true ) {
                 
                 this -> spdata . clear();
//                  picture -> clear();
//                  std::cout << "MYYYYY COUNT: " << count << std::endl;
                 
                 const FLAC__byte * pdata = picture . get_data();

                 for ( unsigned int i = 0; i < picture . get_length(); ++i ) {
                    spdata += pdata[i];
                 }
                 std::cout << "PICTURE!!!" << std::endl;
                 std::cout << "PICTURE: " << picture . get_data_length() << std::endl;
                 picData . insert ( std::pair < ::FLAC__StreamMetadata_Picture_Type, std::string > ( picture . get_type(), spdata ) );
                 std::cout << "FLAC COUNT: " << picData.size() << std::endl;
              }
//               this -> ddata . setPicture ( picData );
//               delete picture; picture = 0;
      }

      void SilentMedia::Codec::FLAC::setDSPParam ( void ) {
         this -> ddata -> setMainParam ( this -> fileSize, this -> totalTime, this -> streamInfo -> get_channels(),
                  this -> streamInfo -> get_sample_rate(), this -> avrBitrate, this -> streamInfo -> get_bits_per_sample() );
      }

      void SilentMedia::Codec::FLAC::setPicture ( std::string coverData ) {
            ::FLAC::Metadata::Picture * picture = new ::FLAC::Metadata::Picture;
         // конвертируем данные в coverData нужные set_data() в const FLAC__byte *
         const FLAC__byte * data = ( const FLAC__byte * ) coverData.c_str();

         if ( picture -> set_data ( data, coverData . size() ) == false ) {
            std::cout << "Error to set picture!" << std::endl;
         }

         // инициализируем итератор в режиме чтение-запись
         iterator -> init ( this -> fileName.c_str(), false, true );

         // Используем ImageMagik дляполучения параметров рисунка ( высота, ширина, глубина, тип )
         Magick::Image * imagick = new Magick::Image;
         Magick::Blob * blob = new Magick::Blob ( coverData.c_str(), coverData.size() );
         imagick -> read ( *blob );

         // составляем mime-тип ( напр. "image/png" )
         std::string mime = "image/" + Utils::Func::toLower ( std::string ( imagick -> magick() ) );

         // записываем эти значения в обект picture
         picture -> set_mime_type ( mime.c_str() );
         picture -> set_width ( imagick -> size() . width() );
         picture -> set_height ( imagick -> size() . height() );
         picture -> set_depth ( imagick -> depth() );

         // запись в файл
         iterator -> insert_block_after ( picture );

         // освобождаем память
         delete imagick;
         delete blob;
         delete picture;
         delete iterator;
      }

      void SilentMedia::Codec::FLAC::closeF ( void ) {
         FLAC__stream_decoder_finish ( this -> decoder );
         FLAC__stream_decoder_flush ( this -> decoder );
      }

      void SilentMedia::Codec::FLAC::flush ( void ) {
         std::cout << "FLAC in flush()" << std::endl;
         /*
         Ставим стражей в исходное положение.
         Возможно следовало как то бы пересоздавать обяек?
         */
         this -> playCheck = false;
         this -> seekCheck = false;

         this -> setCurrSample ( 0 );
//          this -> setSeekPos ( 0 );

//          if ( ( this -> pdecoder ) != NULL ) {
//             FLAC__stream_decoder_finish ( this -> pdecoder );
//             FLAC__stream_decoder_flush ( this -> pdecoder );
//          }
//          if ( ( this -> decoder ) != NULL ) {
//             FLAC__stream_decoder_finish ( this -> decoder );
//             FLAC__stream_decoder_finish ( this -> decoder );
//          }
// FLAC__stream_decoder_flush ( this -> dec );

//          FLAC__stream_decoder_delete ( decoder );
// delete streamInfo;
//          delete decoder;

      }

      void SilentMedia::Codec::FLAC::finish ( void ) {
//          FLAC__stream_decoder_finish ( this -> dec );
//          FLAC__stream_decoder_flush ( this -> dec );
      }

      void SilentMedia::Codec::FLAC::parseMetaData ( void ) {
         iterator -> init ( this -> fileName.c_str(), false, true );
         
            ::FLAC::Metadata::get_streaminfo ( this -> fileName.c_str(), *this -> streamInfo );

         this -> fileSize = Utils::Func::getFileSize ( this -> fileName );
         // применяем сдесь ф-цию FLAC__stream_decoder_get_total_samples() !
         this -> totalTime = ( FLAC__stream_decoder_get_total_samples ( this -> decoder ) / this -> streamInfo -> get_sample_rate() );


         unsigned int size = 0; // переменная для инкремента размера всех найденых блоков
         while ( !iterator -> is_last() ) {
            switch ( iterator -> get_block_type() ) {
               case FLAC__METADATA_TYPE_STREAMINFO:
//                   std::cout << "FLAC__METADATA_TYPE_STREAMINFO" << std::endl;
                  break;
               case FLAC__METADATA_TYPE_PADDING:
//                   std::cout << "FLAC__METADATA_TYPE_PADDING" << std::endl;
                  break;
               case FLAC__METADATA_TYPE_APPLICATION:
//                   std::cout << "FLAC__METADATA_TYPE_APPLICATION" << std::endl;
                  break;
               case FLAC__METADATA_TYPE_SEEKTABLE:
//                   std::cout << "FLAC__METADATA_TYPE_SEEKTABLE" << std::endl;
                  break;
               case FLAC__METADATA_TYPE_VORBIS_COMMENT:
//                   std::cout << "FLAC__METADATA_TYPE_VORBIS_COMMENT" << std::endl;
                  this -> readVorbisComment();
                  break;
               case FLAC__METADATA_TYPE_CUESHEET:
//                   std::cout << "FLAC__METADATA_TYPE_CUESHEET" << std::endl;
                  break;
               case FLAC__METADATA_TYPE_PICTURE:
//                   std::cout << "FLAC__METADATA_TYPE_PICTURE" << std::endl;
                  this -> getPicture();
                  break;
               case FLAC__METADATA_TYPE_UNDEFINED:
//                   std::cout << "FLAC__METADATA_TYPE_UNDEFINED" << std::endl;
                  break;
            }
            iterator -> next(); // движемся далее
            size += iterator -> get_block_length();
      }
       // получаем размер полезных аудио-данных
      this -> streamSize = this -> fileSize - size;
      // получаем средний битрейт
      this -> avrBitrate = this -> streamSize / this -> totalTime * 8;
      this -> setDSPParam();
      }
