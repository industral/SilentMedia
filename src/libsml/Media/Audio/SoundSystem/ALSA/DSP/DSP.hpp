#ifndef _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_ALSA_DSP_DSP_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_ALSA_DSP_DSP_HPP_

// include main
#include <libsml/include.hpp>

// include inheritable class
#include <libsml/Media/Audio/SoundSystem/AbstractDSP.hpp>

// include ALSA
#include <alsa/asoundlib.h>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace ALSA {
          namespace DSP {
            static const string defaultDev = "default";

            class DSP: virtual public AbstractDSP {
              public:
                DSP();
                virtual ~DSP();

                virtual int init(const string &driver);
                virtual bool close();
                virtual bool setAudioParams(const int &channels,
                    const int &sampleRate, const int &bitsPerSample);
                virtual long write(void *buf, const int &bufSize);
              private:
                snd_pcm_t * handle;
            };
          }
        }
      }
    }
  }
}

#endif
