#ifndef _SILENTMEDIA_MEDIA_AUDIO_CODEC_WAVPACK_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_CODEC_WAVPACK_HPP_

/**
 * @see http://www.wavpack.com/lib_use.txt
 */

// main include
#include <libsml/include.hpp>

/*
 * We should include AbstractCodec, AudioProxy.
 */
#include "../AbstractCodec.hpp"
#include "../../AudioProxy.hpp"

#include <wavpack/wavpack.h>

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {
        class WavPack: virtual public AbstractCodec {
          public:
            WavPack();
            virtual ~WavPack();

            virtual bool open(const string &fileId);
            virtual void close(const string &fileId);

            virtual int play(const string &fileId, bool resume = false);
            virtual void stop(const string &fileId);

            virtual float getSeek(const string &fileId);
            virtual void setSeek(const string &fileId, const double &seekVal);
          private:
            // AudioProxy object
            AudioProxy * audioProxy;
            map < string, WavpackContext * > wavPackContextMap;

            map < string, bool > stopMap;
        };
      }
    }
  }
}

#endif
