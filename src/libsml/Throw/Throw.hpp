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

#ifndef _SILENTMEDIA_THROW_THROW_HPP_
#define _SILENTMEDIA_THROW_THROW_HPP_

#include <libsml/include.hpp>

using namespace std;

namespace SilentMedia {
  namespace Throw {
    class File {
      public:
        File() {
        }

        File(const string &message) :
          message(message) {
        }

        string getMessage() const {
          return ("Exception: " + this -> message);
        }
      private:
        string message;
    };

    class FileNotFound: public File {
      public:
        FileNotFound() :
          File("File not found") {
        }

        FileNotFound(const string &fileName) :
          File("File not found: " + fileName) {
        }
      private:
    };

    class NotRegularFile: public File {
      public:
        NotRegularFile() :
          File("Not a file") {
        }

        NotRegularFile(const string &fileName) :
          File(fileName + " not a file") {
        }
      private:
    };

    class ErrorOpenFile: public File {
      public:
        ErrorOpenFile() :
          File("Unable to open file") {
        }

        ErrorOpenFile(const string &fileName) :
          File("Unable to open file: " + fileName) {
        }
      private:
    };

    class ErrorCloseFile: public File {
      public:
        ErrorCloseFile() :
          File("Unable to close file") {
        }

        ErrorCloseFile(const string &fileName) :
          File("Unable to close file: " + fileName) {
        }
      private:
    };

  }
}

#endif
