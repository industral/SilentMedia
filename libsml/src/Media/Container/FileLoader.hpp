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

#ifndef _SILENTMEDIA_MEDIA_CONTAINER_FILELOADER_HPP_
#define _SILENTMEDIA_MEDIA_CONTAINER_FILELOADER_HPP_

#include <libsml/include.hpp>
#include "../../Throw/Throw.hpp"

typedef boost::filesystem::path Path;

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Container {

      enum ContainerType {
        UNKNOW_CONTAINER, NONE_CONTAINER, RIFF_WAVE, RIFF_AVI, RIFF_UNKNOW, OGG
      };

      enum CodecType {
        UNKNOW_CODEC, NONE_CODEC, FLAC, VORBIS, THEORA, WAVPACK
      };

      class FileLoader {
        public:
          /**
           * Constructor.
           */
          FileLoader();

          /**
           * Desctuctor.
           */
          ~FileLoader();

          /**
           * Open media file.
           * param[in] fileName path to media file.
           */
          void open(const string &fileName);

          /**
           * Close file.
           */
          void close();

          /**
           * Get file media container.
           * @return media container type.
           */
          ContainerType getContainer();

          /**
           * Get media codec.
           * @return media codec type.
           */
          CodecType getCodec();

          /**
           * Get file signature (4 first bytes).
           * @return file signature.
           */
          string getFileSignature();
        private:
          /**
           * Read segment of file.
           * @param[in] offset file offset in bytes,
           * @param[in] length size of bytes to read.
           * @return output string of read bytes.
           */
          string readFileSegment(int offset, int length);

          ifstream infile;
          string fileName;

          ContainerType container;
          CodecType codec;
      };
    }
  }
}

#endif
