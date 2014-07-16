#ifndef _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_OSS_DSP_DSP_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_OSS_DSP_DSP_HPP_

// include main
#include <libsml/include.hpp>

// include inheritable class
#include <libsml/Media/Audio/SoundSystem/AbstractDSP.hpp>

// include OSS
#include "/usr/lib/oss/include/sys/soundcard.h"

// C
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace OSS {
          namespace DSP {
            class DSP: virtual public AbstractDSP {
              public:
                DSP();
                virtual ~DSP();

                virtual bool init(const string &driver);
                virtual int close();
                virtual bool setAudioParams(const int &channels,
                    const int &sampleRate, const int &bitsPerSample);
                virtual long write(void *buf, const int &bufSize);
              private:
                int dspDev;
                int input_fd;
            };
          }
        }
      }
    }
  }
}

#endif
