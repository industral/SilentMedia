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
