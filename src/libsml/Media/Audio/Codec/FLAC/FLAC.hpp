#ifndef _SILENTMEDIA_MEDIA_AUDIO_CODEC_FLAC_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_CODEC_FLAC_HPP_

/**
 * http://flac.sourceforge.net/api/classFLAC_1_1Decoder_1_1File.html
 */

// main include
#include <libsml/include.hpp>

/*
 * We should include AbstractCodec, AudioProxy.
 */
#include <libsml/Media/Audio/Codec/AbstractCodec.hpp>
#include <libsml/Media/Audio/AudioProxy.hpp>

// include FLAC++ header files
#include <FLAC++/all.h>
#include "FLACDecoder.hpp"

// ImageMagik (Magick++)
//#include <Magick++.h>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {
        /**
         * FLAC class. Use FLACDecoder class.
         */
        class FLAC: virtual public AbstractCodec {
            friend class FLACDecoder;
          public:
            FLAC();
            virtual ~FLAC();

            virtual bool open(const string &fileId);
            virtual void close(const string &fileId);

            virtual int play(const string &fileId, bool resume = false);
            virtual void stop(const string &fileId);

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
          private:
            // AudioProxy object
            AudioProxy * audioProxy;

            //             ::FLAC::Metadata::Picture * picture;
            std::string spdata;
            map < string, bool > stopMap;

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
            bool templateInitFile(const string &fileId, const string &fileName);
            void parseMetaData(const string &fileId);
            void readVorbisComment(const string &fileId);

            //          void getPicture(void);
            //          double getTotalSamples(void) const {
            //            return this -> totalSamples;
            //          }

            std::map < std::string, std::string > vorbisComm;

            map < string, FLACDecoder * > flacDecoderMap;
            map < string, ::FLAC::Metadata::StreamInfo::StreamInfo * >
                streamInfoMap;

            map < string, ::FLAC::Metadata::SimpleIterator * > iteratorMap;
        };
      }
    }
  }
}

#endif
