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

#include "AO.hpp"

namespace SilentMedia {
  namespace Audio {
    namespace SoundSystem {
      AO::AO() {
        device = NULL;
        default_driver = -1;
        ao_initialize();
      }

      AO::~AO() {
      }

      int AO::init(const string &driver) {
        if (driver.empty()) {
          this -> init();
        } else {
          this -> default_driver = ao_driver_id(driver.c_str());
        }
        return this -> default_driver;
      }

      int AO::init() {
        this -> default_driver = ao_default_driver_id();
        return (this -> default_driver);
      }

      int AO::close() {
        return (ao_close(this -> device));
      }

      void AO::setAudioParams(const int &channels, const int &sampleRate,
          const int &bitsPerSample) {
        format.bits = bitsPerSample;
        format.channels = channels;
        format.rate = sampleRate;
        format.byte_format = AO_FMT_LITTLE;

        device = ao_open_live(default_driver, &format, NULL);
        if (device == NULL) {
          cerr << "Error to set audio parameters" << endl;
        }
      }

      int AO::write(char &buf, const int &bufSize) {
        return (ao_play(device, &buf, bufSize));
      }
    }
  }
}
