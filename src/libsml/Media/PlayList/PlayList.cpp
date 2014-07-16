#include "PlayList.hpp"

namespace SilentMedia {
  namespace Media {
    namespace PlayList {

      PlayList::PlayList() :
        xspf(new XSPF::XSPF) {
      }

      PlayList::~PlayList() {
        delete this -> xspf;
        this -> xspf = NULL;
      }

      bool PlayList::openPlayList(const string &playList) {
        return (this -> xspf -> openPlayList(playList));
      }

      bool PlayList::writePlayList(const string &playList,
          list <TrackInfo> playListData) {
        return (this -> xspf -> writePlayList(playList, playListData));
      }

      bool PlayList::close() {
        return (this -> xspf -> close());
      }

      list <string> PlayList::getPlayList() const {
        return (this -> xspf -> getPlayList());
      }

    }
  }
}
