#ifndef _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_AO_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_AO_HPP_

/**
 * @see libao documentation at http://www.xiph.org/ao/doc/
 */

// main include
#include <libsml/include.hpp>

// include inheritable class
#include <libsml/Media/Audio/SoundSystem/AbstractDSP.hpp>

// include libao header files
#include <ao/ao.h>

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        /**
         * libao sound system class. Need to providing audio output.
         * Support the follow drivers:
         * OSS, ALSA, ESD, Sun audio system, aRts.
         */
        class AO: virtual public AbstractDSP {
          public:
            /*
             * Default constructor.
             */
            AO();

            /**
             * Default destructor.
             */
            virtual ~AO();

            /**
             * Initialized audio system with audio driver.
             * @see list of available audio drivers:
             * http://www.xiph.org/ao/doc/drivers.html
             * @param[in] driver name of audio driver
             * @return > 0 = ID number of the driver,
             * -1 indicates failure. No driver by that name exists.
             * @code
             * audio -> init("alsa");
             * @endcode
             */
            virtual int init(const string& driver);

            virtual bool close();

            /**
             * Initialized audio system with follow audio parameters:
             * @param[in] channels count of channels.
             * @param[in] sampleRate sample rate.
             * @param[in] bitsPerSample bit per sample.
             *
             * This method should be call to set audio system in appropriate status.
             * Otherwise, there are will be noise or wrong interpretation of music.
             *
             * @code
             *   audio -> setAudioParams(2, 44100, 16);
             * @endcode
             */
            virtual bool setAudioParams(const int &channels,
                const int &sampleRate, const int &bitsPerSample);

            /**
             * Write an audio data to audio output.
             * @param[in] buf audio data buffer.
             * @param[in] bufSize size of audio data buffer.
             * @return > 0 = success. 0 indicates failure. The device should be
             * closed.
             */
            virtual long write(void *buf, const int &bufSize);

          private:
            ao_sample_format format;
            ao_device * device;
            int default_driver;
        };
      }
    }
  }
}

#endif
