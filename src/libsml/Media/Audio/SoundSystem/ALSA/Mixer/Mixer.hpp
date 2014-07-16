#ifndef _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_ALSA_MIXER_MIXER_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_ALSA_MIXER_MIXER_HPP_

#include <libsml/all.hpp>

// include ALSA
#include <alsa/asoundlib.h>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace ALSA {
          namespace Mixer {
            class Mixer {
              public:
                /**
                 * Constructor.
                 */
                Mixer();

                /**
                 * Destructor.
                 */
                ~Mixer();

                bool open();
              private:
            };
          }
        }
      }
    }
  }
}

#endif
