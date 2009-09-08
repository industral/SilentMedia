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

#ifndef _SILENTMEDIA_MEDIA_PLAYLIST_TRACKINFO_HPP_
#define _SILENTMEDIA_MEDIA_PLAYLIST_TRACKINFO_HPP_

#include <libsml/include.hpp>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace PlayList {
      class TrackInfo {
        public:
          TrackInfo() :
            trackNum(-1) {
          }

          ~TrackInfo() {
          }

          // set
          void setTitle(const string &title) {
            this -> title = title;
          }

          void setAlbum(const string &album) {
            this -> album = album;
          }

          void setArtist(const string &artist) {
            this -> artist = artist;
          }

          void setTrackNumber(const int &trackNum) {
            this -> trackNum = trackNum;
          }

          void setTrackLocation(const string &trackLocation) {
            this -> trackLocation = trackLocation;
          }

          // get
          string getTitle() const {
            return this -> title;
          }

          string getAlbum() const {
            return this -> album;
          }

          string getArtist() const {
            return this -> artist;
          }

          int getTrackNumber() const {
            return this -> trackNum;
          }

          string getTrackLocation() const {
            return this -> trackLocation;
          }

        private:
          string title;
          string album;
          string artist;
          int trackNum;
          string trackLocation;
      };
    }
  }
}

#endif
