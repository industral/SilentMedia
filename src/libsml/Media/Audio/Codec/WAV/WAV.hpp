#ifndef _SILENTMEDIA_MEDIA_AUDIO_CODEC_WAV_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_CODEC_WAV_HPP_

// main include
#include <libsml/include.hpp>

/*
 * We should include AbstractCodec, AudioProxy.
 */
#include "../AbstractCodec.hpp"
#include "../../AudioProxy.hpp"

// include WAV header
#include "WAVInfo.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {
        class WAV: virtual public AbstractCodec {
          public:
            WAV();
            virtual ~WAV();

            virtual bool open(const string &fileId);
            virtual void close(const string &fileId);

            virtual int play(const string &fileId, bool resume = false);
            virtual void stop(const string &fileId);

            virtual float getSeek(const string &fileId);
            virtual void setSeek(const string &fileId, const double &seekVal);
          private:
            // AudioProxy object
            AudioProxy * audioProxy;

            map < string, WAVInfo * > wavInfoMap;
            map < string, double > offsetPositionMap;
            map < string, ifstream * > inputFDMap;
            map < string, bool > stopMap;
        };
      }
    }
  }
}

#endif
