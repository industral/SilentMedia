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

#ifndef __SS_SILENTMEDIA_FLAC_HPP_
#define __SS_SILENTMEDIA_FLAC_HPP_

#include <SML/sml.hpp>
#include <SML/Audio/Codec/AbstractCodec.hpp>
#include <Magick++.h> // ImageMagik (Magick++)

namespace SilentMedia {
   class DecodedData;
   /// Клас для декодирования FLAC-файлов
   namespace Codec {
      class FLAC : virtual public AbstractCodec {
      public:
         FLAC ( void );
         virtual ~FLAC ( void );

         // virtual
         virtual bool init ( std::string inputFile );
         virtual void play ( void );
         virtual unsigned long int getCurrSeekPos ( void ) const;
         virtual void setSeekPos ( double val );
         virtual void closeF ( void );
         virtual void flush ( void );
         virtual void finish ( void );

         // public
         /// Установить обложку
         /** Устанавливает обложку изменением внутренных данных в файле. Файл должен быть доступный в режиме чтение/запись */
         /// @param coverData непосредственно данные рисунка
         void setPicture ( std::string coverData );
         // public for internal use
         inline void setCurrSample ( FLAC__uint64 curr_sample ) { this -> curr_sample = curr_sample; }
         inline FLAC__uint64 getCurrSample ( void ) const { return this -> curr_sample; }
         int getdspDev ( void ) { return ( this -> dspDev ); }
      private:
//             ::FLAC::Metadata::Picture * picture;
            std::string spdata;
            
//          template<class T>
//                class Proxy {
//                   public:
//                      T value;
// 
//                      Proxy<T>& operator=(const T& v) {
//                         value = v;
// //                         std::cout << value << std::endl;
//                         std::cout << "_PROXY_PICTURE: " << value . get_data_length() << std::endl;
//                      }
//                };
               
         // private
         bool templateInitFile ( FLAC__StreamDecoder * dec );
         void parseMetaData ( void );
         void readVorbisComment ( void );
         void getPicture ( void );
         void setDSPParam ( void );
         double getTotalSamples ( void ) const { return this -> totalSamples; }

         DecodedData * ddata;
         std::map < std::string, std::string > vorbisComm;
         std::string fileName;
         /*
         Почему не работает FLAC::Decoder::File *encoder= new FLAC::Decoder::File()?
           Приходится применять FLAC__stream_decoder_new()
         */
         FLAC__StreamDecoder * decoder; // указатель для инициализации
         FLAC__StreamDecoder * pdecoder; // указатель для воспроизвдения
            ::FLAC::Metadata::SimpleIterator * iterator;
            int dspDev;

         // стражи
         bool playCheck;
         bool seekCheck;

         /*
         используем double для totalSamples а не FLAC__uint64 чтобы не использовать потом static_cast,
         так как переменная используется для деления
         см. int libssoss::FlacDecode::getCurrSeekPos ( void ) const
         */

         int fileSize;
         int streamSize;
         double avrBitrate;
         double totalSamples;
         int totalTime;
         FLAC__uint64 curr_sample;
         FLAC__uint64 * position;
            ::FLAC::Metadata::StreamInfo::StreamInfo * streamInfo;
   };
}
}

#endif
