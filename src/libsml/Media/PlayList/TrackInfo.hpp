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
