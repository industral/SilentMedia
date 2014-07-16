#include "DSP.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace OSS {
          namespace DSP {

            DSP::DSP() :
              dspDev(-1), input_fd(-1) {
            }

            DSP::~DSP() {
              //              if (close(this -> dspDev) != 0) {
              //                perror("dsp_fd");
              //              }
            }

            int DSP::init(const string &driver) {
              string defaultDev = "/dev/dsp";

              if (!driver.empty()) {
                defaultDev = driver;
              }

              //TODO: open check.
              this -> dspDev = open(defaultDev.c_str(), O_WRONLY);

              return true;
            }

            bool DSP::close() {
              return (::close(this -> dspDev));
            }

            bool DSP::setAudioParams(const int &channels,
                const int &sampleRate, const int &bitsPerSample) {
              int format = AFMT_S16_NE; // bit per sample

              // NOTE: DO NOT set channel as short!
              int audioChannels = channels;
              int audioSampleRate = sampleRate;
              //              int audioBitsPerSample = bitsCPerSample;

              if (ioctl(this -> dspDev, SNDCTL_DSP_SETFMT, &format) == -1) {
                perror("SNDCTL_DSP_SETFMT");
                return false;
              }
              //
              if (ioctl(this -> dspDev, SNDCTL_DSP_CHANNELS, &audioChannels)
                  == -1) {
                perror("SNDCTL_DSP_CHANNELS");
                return false;
              }

              if (ioctl(this -> dspDev, SNDCTL_DSP_SPEED, &audioSampleRate)
                  == -1) {
                perror("SNDCTL_DSP_SPEED");
                return false;
              }
              return true;
            }

            long DSP::write(void *buf, const int &bufSize) {
              return (::write(dspDev, buf, bufSize));
            }

          }
        }
      }
    }
  }
}
