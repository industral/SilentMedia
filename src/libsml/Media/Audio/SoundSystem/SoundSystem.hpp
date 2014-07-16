#ifndef _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_SOUNDSYSTEM_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_SOUNDSYSTEM_HPP_

// main include
#include <libsml/include.hpp>

#include "AbstractDSP.hpp"

// include available sound systems

#ifdef LIBAO
#include "libao/AO.hpp"
#endif

#ifdef ALSA
#include "ALSA/DSP/DSP.hpp"
#endif

#ifdef OSS
#include "OSS/DSP/DSP.hpp"
#endif

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        class SoundSystem {
          public:
            SoundSystem();
            ~SoundSystem();

            static SoundSystem * Instance();

            int init(const string &soundDriver, const string &driver);
            int init();
            int close();
            void setAudioParams(const int &channels, const int &sampleRate,
                const int &bitsPerSample);
            int write(void *buf, const int &bufSize);

          private:
            // Singleton variable
            static SoundSystem * _soundSystem;

            AbstractDSP * dsp;
        };
      }
    }
  }
}

#endif
