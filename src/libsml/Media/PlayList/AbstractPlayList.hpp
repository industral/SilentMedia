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
