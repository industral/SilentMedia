#include "AO.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        AO::AO() {
          device = NULL;
          default_driver = -1;
          ao_initialize();
        }

        AO::~AO() {
          ao_shutdown();
        }

        int AO::init(const string &driver) {
          if (driver.empty()) {
            this -> default_driver = ao_default_driver_id();
          } else {
            this -> default_driver = ao_driver_id(driver.c_str());
          }
          return this -> default_driver;
        }

        bool AO::close() {
          return (ao_close(this -> device));
        }

        bool AO::setAudioParams(const int &channels, const int &sampleRate,
            const int &bitsPerSample) {
          format.bits = bitsPerSample;
          format.channels = channels;
          format.rate = sampleRate;
          format.byte_format = AO_FMT_LITTLE;

          device = ao_open_live(default_driver, &format, NULL);
          if (device == NULL) {
            cerr << "Error to set audio parameters" << endl;
            return false;
          }
          return true;
        }

        long AO::write(void *buf, const int &bufSize) {
          return (ao_play(device, (char *) buf, bufSize));
        }
      }
    }
  }
}
