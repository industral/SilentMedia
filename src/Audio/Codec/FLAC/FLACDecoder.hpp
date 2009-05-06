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

#ifndef _SILENTMEDIA_FLACDECODER_HPP_
#define _SILENTMEDIA_FLACDECODER_HPP_

#include "FLAC.hpp"

namespace SilentMedia {
  namespace Audio {
    namespace Codec {
      class FLAC;

      /**
       * FLAC Decoder class. Inheritance from ::FLAC::Decoder::File.
       * @see http://flac.sourceforge.net/api/classFLAC_1_1Decoder_1_1File.html
       */
      class FLACDecoder: public ::FLAC::Decoder::File {
        public:
          FLACDecoder();

          virtual FLAC__StreamDecoderWriteStatus write_callback(
              const FLAC__Frame * frame, const FLAC__int32 * const buf[]);

          virtual void error_callback(::FLAC__StreamDecoderErrorStatus status);

          void setFileId(const string &fileId);

        private:
          FLAC * flacObj;
          string fileId;
      };
    }
  }
}

#endif
