#include "SoundSystem.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        SoundSystem * SoundSystem::_soundSystem = NULL;

        SoundSystem::SoundSystem() {
        }

        SoundSystem::~SoundSystem() {
        }

        SoundSystem * SoundSystem::Instance() {
          if (_soundSystem == NULL) {
            _soundSystem = new SoundSystem();
          }
          return _soundSystem;
        }

        int SoundSystem::init(const string &soundDriver, const string &driver) {
          if (soundDriver.compare("ALSA") == 0) {
            #ifdef ALSA
            this -> dsp = new ALSA::DSP::DSP();
            #endif
          } else if (soundDriver.compare("OSS") == 0) {
            #ifdef OSS
            this -> dsp = new OSS::DSP::DSP();
            #endif
          } else if (soundDriver.compare("AO") == 0) {
            #ifdef LIBAO
            this -> dsp = new AO();
            std::cout << "AO" << std::endl;
            #endif
          } else {
            return false;
          }
          return (this -> dsp -> init(driver));
        }

        int SoundSystem::close() {
          return (this -> dsp -> close());
        }

        void SoundSystem::setAudioParams(const int &channels,
            const int &sampleRate, const int &bitsPerSample) {
          this -> dsp -> setAudioParams(channels, sampleRate, bitsPerSample);
        }

        int SoundSystem::write(void *buf, const int &bufSize) {
          return (this -> dsp -> write(buf, bufSize));
        }

      }
    }
  }
}
