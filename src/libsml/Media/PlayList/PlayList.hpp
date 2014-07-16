#ifndef _SILENTMEDIA_MEDIA_PLAYLIST_PLAYLIST_HPP_
#define _SILENTMEDIA_MEDIA_PLAYLIST_PLAYLIST_HPP_

#include <libsml/Media/PlayList/AbstractPlayList.hpp>
#include <libsml/Media/PlayList/XSPF/XSPF.hpp>

namespace SilentMedia {
  namespace Media {
    namespace PlayList {
      class PlayList: virtual public AbstractPlayList {
        public:
          PlayList();
          virtual ~PlayList();

          virtual bool openPlayList(const string &playList);
          virtual bool writePlayList(const string &playList,
              list <TrackInfo> playListData);

          virtual bool close();
          virtual list <string> getPlayList() const;
        private:
          XSPF::XSPF * xspf;
      };
    }
  }
}

#endif
