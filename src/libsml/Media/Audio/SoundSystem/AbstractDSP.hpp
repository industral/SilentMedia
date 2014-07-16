#ifndef _SILENTMEDIA_MEDIA_AUDIO_SOUNDYSTEM_ABSTRACTDSP_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_SOUNDYSTEM_ABSTRACTDSP_HPP_

#include <libsml/include.hpp>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        class AbstractDSP {
          public:
            /*
             * Default constructor.
             */
            AbstractDSP() {
            }

            virtual ~AbstractDSP() {
            }

            virtual int init(const string &driver) = 0;
            virtual bool close() = 0;
            virtual bool setAudioParams(const int &channels,
                const int &sampleRate, const int &bitsPerSample) = 0;
            virtual long write(void *buf, const int &bufSize) = 0;

            /**
             * Get a list of available devices for audio driver.
             * e.g. "/dev/dsp" or "default".
             * return a list of available devices.
             */
//            virtual list <string> getAvailableDriverList() = 0;

          protected:
          private:
        };
      }
    }
  }
}

#endif
