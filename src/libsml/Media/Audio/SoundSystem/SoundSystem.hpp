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

#ifndef _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_SOUNDSYSTEM_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_SOUNDSYSTEM_HPP_

// main include
#include <libsml/include.hpp>

/*
 * We should include AbstractSoundSystem
 */
#include "AbstractSoundSystem.hpp"

// include available sound systems
#include "libao/AO.hpp"
#include "ALSA/DSP/DSP.hpp"

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        class SoundSystem: virtual public AbstractSoundSystem {
          public:
            SoundSystem();
            virtual ~SoundSystem();

            static SoundSystem * Instance();

            // Inheritance methods
            virtual int init(const string &driver);
            virtual int init();
            virtual int close();
            virtual void setAudioParams(const int &channels,
                const int &sampleRate, const int &bitsPerSample);
            virtual int write(void *buf, const int &bufSize);

          private:
            // Singleton variable
            static SoundSystem * _soundSystem;

            AbstractSoundSystem * dsp;
        };
      }
    }
  }
}

#endif
