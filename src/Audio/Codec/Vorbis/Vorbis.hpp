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

/*
 * http://www.xiph.org/vorbis/doc/vorbisfile/index.html
 */

#ifndef _SILENTMEDIA_VORBIS_HPP_
#define _SILENTMEDIA_VORBIS_HPP_

// main include
#include <libsml/include.hpp>

// include boost
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

/*
 * We should include AbstractCodec, AudioProxy.
 */
#include <libsml/Audio/Codec/AbstractCodec.hpp>
#include <libsml/Audio/AudioProxy.hpp>

// Vorbis include
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

using namespace std;

namespace SilentMedia {
  namespace Audio {
    namespace Codec {
      class Vorbis: virtual public AbstractCodec {
        public:
          Vorbis();
          virtual ~Vorbis();

          virtual bool open(const string &fileId);
          virtual void close(const string &fileId);

          virtual int play(const string &fileId, bool resume = false);
          virtual void stop(const string &fileId);

          virtual float getSeek(const string &fileId);
          virtual void setSeek(const string &fileId, const double &seekVal);
        private:
          // AudioProxy object
          AudioProxy * audioProxy;

          // Vorbis File map
          map < string, OggVorbis_File > vorbisFileMap;

          // Vorbis Info map
          map < string, vorbis_info * > vorbisInfoMap;

          map < string, bool > stopMap;

          void readVorbisComment(const string &fileId);
      };
    }
  }
}

#endif
