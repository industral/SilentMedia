#include "DSP.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace ALSA {
          namespace DSP {

            DSP::DSP() {
            }

            DSP::~DSP() {
            }

            int DSP::init(const string &driver) {
              string defaultDevice = defaultDev;

              if (!driver.empty()) {
                defaultDevice = driver;
              }

              int err = -1;
              if ((err = snd_pcm_open(&this -> handle, defaultDev.c_str(),
                  SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
                cout << "Playback open error: " << snd_strerror(err) << endl;
                exit(EXIT_FAILURE);
              }
              return true;
            }

            bool DSP::close() {
              return true; // TODO:fix
            }

            bool DSP::setAudioParams(const int &channels,
                const int &sampleRate, const int &bitsPerSample) {
              _snd_pcm_format format;

              cout << "SET AUDIO PARAMS: " << endl;

              if (bitsPerSample == 8) {
                format = SND_PCM_FORMAT_S8;
              } else if (bitsPerSample == 16) {
                format = SND_PCM_FORMAT_S16;
              } else if (bitsPerSample == 24) {
                format = SND_PCM_FORMAT_S24;
              } else if (bitsPerSample == 32) {
                format = SND_PCM_FORMAT_S32;
              }

              int err = NULL;

              if ((err = snd_pcm_set_params(this -> handle, format,
                  SND_PCM_ACCESS_RW_INTERLEAVED, channels, sampleRate, 0,
                  500000)) < 0) {
                cerr << "Playback open error: " << snd_strerror(err) << endl;
                return false;
              }
              return true;
            }

            long DSP::write(void *buf, const int &bufSize) {
              return (snd_pcm_writei(this -> handle, buf, bufSize / 4));
            }

//            list <string> getAvailableDriverList() {
//
//            }

          }
        }
      }
    }
  }
}
