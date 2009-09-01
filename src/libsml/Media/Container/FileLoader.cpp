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

#include "FileLoader.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Container {

      // --------------------------------------------------------------------
      // Public methods
      // --------------------------------------------------------------------

      FileLoader::FileLoader() {
      }

      FileLoader::~FileLoader() {
      }

      void FileLoader::open(const string &fileName) {
        this -> fileName = fileName;

        Path path(fileName);

        if (!boost::filesystem::exists(path)) {
          throw SilentMedia::Throw::FileNotFound(fileName);
        }
        if (!boost::filesystem::is_regular_file(path)) {
          throw SilentMedia::Throw::NotRegularFile(fileName);
        }

        this -> infile.open(fileName.c_str(), ios::binary);

        if (!this -> infile.is_open()) {
          throw SilentMedia::Throw::ErrorOpenFile(fileName);
        }

      }

      void FileLoader::close() {
        this -> infile.close();

        if (this -> infile.is_open()) {
          throw SilentMedia::Throw::ErrorCloseFile(this -> fileName);
        }
      }

      ContainerType FileLoader::getContainer() {
        // offset = 0, length = 4
        string signature = this -> readFileSegment(0, 4);
        if (signature.compare("OggS")) {
          this -> container = OGG;
        } else if (signature.compare("RIFF")) {
          // RIFF have WAVE and AVI, so clarify it
          string containerDetail = this -> readFileSegment(8, 4);
          if (containerDetail.compare("WAVE")) {
            this -> container = RIFF_WAVE;
          } else if (containerDetail.compare("AVI")) {
            this -> container = RIFF_AVI;
          } else {
            this -> container = RIFF_UNKNOW;
          }
        }
        return this -> container;
      }

      //TODO: Need to return 2 codecs, audio & video
      CodecType FileLoader::getCodec() {
        /* OGG */
        if (this -> container == OGG) {
          // offset = 29, length = 6
          string mediaCodec = this -> readFileSegment(29, 6);
          if (mediaCodec.compare("vorbis")) {
            this -> codec = VORBIS;
          } else if (mediaCodec.compare("theora")) {
            this -> codec = THEORA;
          }
        }
        return this -> codec;
      }

      string FileLoader::getFileSignature() {
        return this -> readFileSegment(0, 4);
      }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      string FileLoader::readFileSegment(int offset, int length) {
        if (offset > 0) {
          this -> infile.seekg(offset, ios_base::beg);
        }

        char buf[length + 1];
        this -> infile.read(buf, length);
        buf[length] = 0;

        return buf;
      }
    }
  }
}
