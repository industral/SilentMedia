#ifndef _SILENTMEDIA_MEDIA_AUDIO_CODEC_ABSTRACTCODEC_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_CODEC_ABSTRACTCODEC_HPP_

#include <libsml/include.hpp>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {
        class AbstractCodec {
          public:
            AbstractCodec() {
            }

            virtual ~AbstractCodec() {
            }

            virtual bool open(const string &fileId) = 0;
            virtual void close(const string &fileId) = 0;

            virtual int play(const string &fileId, bool resume = false) = 0;
            virtual void stop(const string &fileId) = 0;

            virtual float getSeek(const string &fileId) = 0;
            virtual void
                setSeek(const string &fileId, const double &seekVal) = 0;
          protected:
        };
      }
    }
  }
}

#endif
