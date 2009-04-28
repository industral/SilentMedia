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

/**
 * @file Util class. Provide main function.
 */

#include "Func.hpp"

namespace SilentMedia {
  namespace Utils {
    string Func::toUpper(string strToConvert) {
      for (unsigned int i = 0; i != strToConvert.length(); ++i) {
        strToConvert[i] = toupper(strToConvert[i]);
      }
      return strToConvert;
    }

    string Func::toLower(string strToConvert) {
      for (unsigned int i = 0; i != strToConvert.length(); ++i) {
        strToConvert[i] = tolower(strToConvert[i]);
      }
      return strToConvert;
    }

    string Func::getFileNamefromFullPath(const string& str) {
      return (str.substr((str.find_last_of("/")) + 1));
    }

    string Func::getFolderNamefromFullPath(const string& str) {
      return (str.substr(0, str.find_last_of("/")));
    }

    string Func::getFileExtension(const string& fileName) {
      string file = getFileNamefromFullPath(fileName);
      return (file.substr((file.find_last_of(".")) + 1));
    }

    long Func::getFileSize(string inputFile) {
      long begin = -1;
      long end = -1;
      ifstream myfile(inputFile.c_str());
      begin = myfile.tellg();
      myfile.seekg(0, ios::end);
      end = myfile.tellg();
      myfile.close();

      return (end - begin);
    }

    bool Func::checkFileAvailable(const string &fileName) {
      ifstream fin(fileName.c_str(), ifstream::in);
      if (fin.is_open()) {
        // file exist
        return true;
      }
      // file did not exist
      return false;
    }

  }
}
