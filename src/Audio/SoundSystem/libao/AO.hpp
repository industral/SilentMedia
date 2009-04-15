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

#include <include.hpp>
#include <Audio/SoundSystem/AbstractSoundSystem.hpp>
#include <ao/ao.h>

namespace SilentMedia {
  namespace Audio {
    namespace SoundSystem {

      class AO: public AbstractSoundSystem {
        public:
          AO();
          virtual ~AO();

          /**
           * Main sound system initialized method. Use it to set output audio driver.
           * @param driver libao driver. See http://www.xiph.org/ao/doc/drivers.html
           * @return > 1 in success, < 1 in false.
           */
          virtual bool init(string driver);
          virtual void setAudioParams(int channels, int sampleRate,
              int bitsPerSample);
          virtual int play(char * buf, const int bufSize);

        private:
          ao_sample_format format;
          ao_device * device;
          int default_driver;
      };
    }
  }
}

#endif
