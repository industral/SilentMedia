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

#ifndef _SILENTMEDIA_FLAC_HPP_
#define _SILENTMEDIA_FLAC_HPP_

// main include
#include <libsml/include.hpp>

// include Util class
#include <libsml/Utils/Func/Func.hpp>

/*
 * We should include AbstractCodec, AudioProxy.
 */
#include <libsml/Audio/Codec/AbstractCodec.hpp>
#include <libsml/Audio/AudioProxy.hpp>

// include FLAC++ header files
#include <FLAC++/all.h>

// ImageMagik (Magick++)
//#include <Magick++.h>

using namespace std;

namespace SilentMedia {
  namespace Audio {
    namespace Codec {
      class FLAC: virtual public AbstractCodec {
        public:
          FLAC();
          virtual ~FLAC();

          virtual bool open(const string &fileId);
          virtual void play(const string &fileId, bool resume = false);
          virtual void close(const string &fileId);

          virtual float getSeek(const string &fileId);
          virtual void setSeek(const string &fileId, const double &seekVal);

          // public
          /// Установить обложку
          /** Устанавливает обложку изменением внутренных данных в файле. Файл должен быть доступный в режиме чтение/запись */
          /// @param coverData непосредственно данные рисунка
          //          void setPicture(std::string coverData);
          // public for internal use
          //          inline void setCurrSample(FLAC__uint64 curr_sample) {
          //            this -> curr_sample = curr_sample;
          //          }
          //          inline FLAC__uint64 getCurrSample(void) const {
          //            return this -> curr_sample;
          //          }
          AudioProxy * getAudioProxy() {
            return (this -> audioProxy);
          }
        private:
          // AudioProxy object
          AudioProxy * audioProxy;

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
          bool templateInitFile(FLAC__StreamDecoder * dec,
              const string &fileName);
          void parseMetaData(const string &fileId);
          //          void readVorbisComment(void);
          //          void getPicture(void);
          //          double getTotalSamples(void) const {
          //            return this -> totalSamples;
          //          }

          //          DecodedData * ddata;
          std::map < std::string, std::string > vorbisComm;
          /*
           Почему не работает FLAC::Decoder::File *encoder= new FLAC::Decoder::File()?
           Приходится применять FLAC__stream_decoder_new()
           */
          //          FLAC__StreamDecoder * decoder; // указатель для инициализации
          //          FLAC__StreamDecoder * pdecoder; // указатель для воспроизвдения

          map < string, FLAC__StreamDecoder * > decoderMap;
          map < string, ::FLAC::Metadata::StreamInfo::StreamInfo * >
              streamInfoMap;

          map < string, ::FLAC::Metadata::SimpleIterator * > iteratorMap;

          // стражи
          map < string, bool > playCheckMap;
          map < string, bool > seekCheckMap;

          /*
           используем double для totalSamples а не FLAC__uint64 чтобы не использовать потом static_cast,
           так как переменная используется для деления
           см. int libssoss::FlacDecode::getCurrSeekPos ( void ) const
           */

          map < string, double > totalSamples;
          FLAC__uint64 curr_sample;
          FLAC__uint64 * position;

      };
    }
  }
}

#endif
