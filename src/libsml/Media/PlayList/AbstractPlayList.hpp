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

#ifndef _SILENTMEDIA_MEDIA_PLAYLIST_ABSTRACTPLAYLIST_HPP_
#define _SILENTMEDIA_MEDIA_PLAYLIST_ABSTRACTPLAYLIST_HPP_

#include <libsml/include.hpp>
#include "TrackInfo.hpp"

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace PlayList {
      class AbstractPlayList {
        public:

          /**
           * Default constructor.
           */
          AbstractPlayList() {
          }

          /**
           * Default destructor.
           */
          virtual ~AbstractPlayList() {
          }

          virtual bool openPlayList(const string &playList) = 0;
          virtual bool writePlayList(const string &playList,
              list <TrackInfo> playListData) = 0;

          virtual bool close() = 0;

          virtual list <string> getPlayList() const = 0;
        private:
        protected:
      };
    }
  }
}

#endif
