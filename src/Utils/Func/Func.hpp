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



#ifndef _SILENTMEDIA_FUNC_HPP_
#define _SILENTMEDIA_FUNC_HPP_

#include <include.hpp>

namespace SilentMedia {
  namespace Utils {
    class Func {
      public:
        /**
         * Return new string in upper case.
         * @param strToConvert string that should be converted.
         * @return string in upper case.
         */
        static string toUpper(string strToConvert);

        /**
         * Return new string in lower case.
         * @param strToConvert string that should be converted.
         * @return string in lower case.
         */
        static string toLower(string strToConvert);

        /**
         * Return file name from full path.
         */
        static string getFileNamefromFullPath(const string& str);

        /**
         * Return path without file name.
         */
        static string getFolderNamefromFullPath(const string& str);

        /**
         * Return file extension from file. For example: file.wav => wav,
         * /data/music/file.ogg => ogg.
         * @param fileName file name.
         * @return file extension.
         */
        static string getFileExtension(const string& fileName);

        /**
         * Return file size.
         * @param inputFile file name that should be count.
         * @return file size in bytes.
         */
        static long int getFileSize(string inputFile);
    };
  }
}

#endif
