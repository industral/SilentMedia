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

#ifndef _SILENTMEDIA_VORBIS_HPP_
#define _SILENTMEDIA_VORBIS_HPP_

#include <include.hpp>

#include <Audio/Codec/AbstractCodec.hpp>
#include <Audio/SoundSystem/libao/AO.hpp>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

using namespace std;

namespace SilentMedia {
  namespace Audio {
    namespace Codec {
      class Vorbis: virtual public AbstractCodec {
        public:
          Vorbis(void);
          virtual ~Vorbis(void);

          virtual void open(string fileName, string fileId);
          virtual void play(string fileId);
          virtual void pause(string fileId);
          virtual void stop(string fileId);
          virtual void close(string fileId);

          virtual float getSeek(string fileId);
          virtual void setSeek(string fileId, float seekVal);
        private:
          void readVorbisComment(void);

          OggVorbis_File vf;
          OggVorbis_File pvf;

          //        DecodedData * ddata;
          int dspDev;

          vorbis_info *vi;

          double length;
          double seekPos;
          bool seek;
          std::string fileName;
      };
    }
  }
}

#endif
