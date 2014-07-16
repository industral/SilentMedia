/**
 * @see http://xspf.org/
 * @see http://libspiff.sourceforge.net/
 * @see http://libspiff.sourceforge.net/doc/html/
 * @see http://sourceforge.net/projects/libspiff
 */

#ifndef _SILENTMEDIA_MEDIA_PLAYLIST_XSPF_XSPF_HPP_
#define _SILENTMEDIA_MEDIA_PLAYLIST_XSPF_XSPF_HPP_

// main include
#include <libsml/include.hpp>

// include Interface
#include <libsml/Media/PlayList/AbstractPlayList.hpp>

#include <libsml/Media/Container/FileLoader.hpp>

// include libXSPF header
#include <libsml/Media/PlayList/XSPF/libXSPF.hpp>

// track information bean
#include <libsml/Media/PlayList/TrackInfo.hpp>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace PlayList {
      namespace XSPF {
        class XSPF: virtual public AbstractPlayList {
          public:
            XSPF();
            virtual ~XSPF();

            virtual bool openPlayList(const string &playList);
            virtual bool writePlayList(const string &playList, list <TrackInfo> playListData);
            virtual bool close();
            virtual list <string> getPlayList() const;

          private:
            libXSPF* libxspf;
            Xspf::XspfReader reader;
        };
      }
    }
  }
}

#endif
