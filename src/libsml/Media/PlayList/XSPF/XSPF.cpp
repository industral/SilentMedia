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

#include "XSPF.hpp"

using namespace ::SilentMedia::Media;

namespace SilentMedia {
  namespace Media {
    namespace PlayList {
      namespace XSPF {

        // --------------------------------------------------------------------
        // Public methods
        // --------------------------------------------------------------------

        XSPF::XSPF() :
          libxspf(new libXSPF) {
        }

        XSPF::~XSPF() {
          delete this -> libxspf;
          this -> libxspf = NULL;
        }

        bool XSPF::open(const string &playList) {
          // Check file on any throw
          Container::FileLoader fileLoader;
          fileLoader.open(playList);
          fileLoader.close();

          Xspf::XspfReaderCallback  * callback = new libXSPF();
          XML_Char const * const baseUri = _PT("http://d-industrial.com");
          const int res =
              reader.parseFile(_PT(playList.c_str()), callback, baseUri);

          return (res == Xspf::XSPF_READER_SUCCESS) ? true : false;
        }

        bool XSPF::close() {

        }

        list<map<string, string> > XSPF::getPlayListMap() {

        }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      }
    }
  }
}
