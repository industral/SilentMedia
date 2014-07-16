#include "Mixer.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace ALSA {
          namespace Mixer {

            // --------------------------------------------------------------------
            // Public methods
            // --------------------------------------------------------------------

            Mixer::Mixer() {
            }

            Mixer::~Mixer() {
            }

            bool Mixer::open() {
//              int retCode = -1;

              snd_mixer_t *mixer_handle;
              //              snd_ctl_t *ctl_handle;
              //
              //              if (snd_ctl_open(&ctl_handle, "default", SND_CTL_NONBLOCK) < 0) {
              //                cerr << "Error: snd_ctl_open()" << endl;
              //              }

              if (snd_mixer_open(&mixer_handle, 0) < 0) {
                cerr << "Error: in snd_mixer_open()" << endl;
                return false;
              }

              if (snd_mixer_attach(mixer_handle, "default") < 0) {
                cerr << "Error: in snd_mixer_attach()" << endl;
                return false;
              }

              snd_mixer_load(mixer_handle);
              cout << snd_mixer_get_count(mixer_handle) << endl;

              return true;
            }

          }
        }
      }
    }
  }
}
