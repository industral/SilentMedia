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

            int DSP::close() {
              ::close(this -> dspDev);
            }

            void DSP::setAudioParams(const int &channels,
                const int &sampleRate, const int &bitsPerSample) {
              int format = AFMT_S16_NE; // bit per sample

              // NOTE: DO NOT set channel as short!
              int audioChannels = channels;
              int audioSampleRate = sampleRate;
              //              int audioBitsPerSample = bitsCPerSample;

              if (ioctl(this -> dspDev, SNDCTL_DSP_SETFMT, &format) == -1) {
                perror("SNDCTL_DSP_SETFMT");
                //                return false;
              }
              //
              if (ioctl(this -> dspDev, SNDCTL_DSP_CHANNELS, &audioChannels)
                  == -1) {
                perror("SNDCTL_DSP_CHANNELS");
                //                return false;
              }

              if (ioctl(this -> dspDev, SNDCTL_DSP_SPEED, &audioSampleRate)
                  == -1) {
                perror("SNDCTL_DSP_SPEED");
                //                return false;
              }

              //              int frag = 8;
              //              ioctl(this -> dspDev, SNDCTL_DSP_SETFRAGMENT, &frag);

              int policy = 3;
              ioctl(this -> dspDev, SNDCTL_DSP_POLICY, &policy);

            }

            int DSP::write(void *buf, const int &bufSize) {
              ::write(dspDev, buf, bufSize);
            }

          }
        }
      }
    }
  }
}
