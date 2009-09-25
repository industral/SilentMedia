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
            this -> dsp = new ALSA::DSP::DSP();
          } else if (soundDriver.compare("OSS") == 0) {
            this -> dsp = new OSS::DSP::DSP();
          } else if (soundDriver.compare("AO") == 0) {
            this -> dsp = new AO();
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
