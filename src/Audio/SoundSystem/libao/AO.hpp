/*******************************************************************************
 * Copyright (c) 2009, Alex Ivasyuv                                            *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *                                                                             *
 * 1. Redistributions of source code must retain the above copyright           *
 *    notice, this list of conditions and the following disclaimer.            *
 * 2. Redistributions in binary form must reproduce the above copyright        *
 *    notice, this list of conditions and the following disclaimer in the      *
 *    documentation and/or other materials provided with the distribution.     *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY Alex Ivasyuv ''AS IS'' AND ANY                 *
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE      *
 * DISCLAIMED. IN NO EVENT SHALL Alex Ivasyuv BE LIABLE FOR ANY DIRECT,        *
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES          *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;*
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF    *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           *
 ******************************************************************************/

#ifndef _SILENTMEDIA_AO_HPP_
#define _SILENTMEDIA_AO_HPP_

/**
 * @see libao documentation at http://www.xiph.org/ao/doc/
 */

// main include
#include <libsml/include.hpp>

// include inheritable class
#include <libsml/Audio/SoundSystem/AbstractSoundSystem.hpp>

// include libao header files
#include <ao/ao.h>

namespace SilentMedia {
  namespace Audio {
    namespace SoundSystem {
      /**
       * libao sound system class. Need to providing audio output.
       * Support the follow drivers:
       * OSS, ALSA, ESD, Sun audio system, aRts.
       */
      class AO: public AbstractSoundSystem {
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

          /**
           * Initialized audio system with default audio driver.
           * @see AO::init(const string&)
           */
          virtual int init();

          virtual int close();

          /**
           * Initialized audio system with follow audio parameters:
           * @param[in] channels count of channels.
           * @param[in] sampleRate sample rate.
           * @param[in] bit per sample.
           *
           * This method should be call to set audio system in appropriate status.
           * Otherwise, there are will be noise or wrong interpretation of music.
           *
           * @code
           * audio -> setAudioParams(2, 44100, 16);
           * @endcode
           */
          virtual void setAudioParams(const int &channels,
              const int &sampleRate, const int &bitsPerSample);

          /**
           * Write an audio data to audio output.
           * @param[in] buf audio data buffer.
           * @param[in] bufSize size of audio data buffer.
           * @return > 0 = success. 0 indicates failure. The device should be
           * closed.
           */
          virtual inline int write(char &buf, const int &bufSize);

        private:
          ao_sample_format format;
          ao_device * device;
          int default_driver;
      };
    }
  }
}

#endif
