/**
 * @see http://libspiff.sourceforge.net/
 */

#ifndef _SILENTMEDIA_MEDIA_PLAYLIST_XSPF_LIBXSPF_HPP_
#define _SILENTMEDIA_MEDIA_PLAYLIST_XSPF_LIBXSPF_HPP_

// main include
#include <libsml/include.hpp>

// include XSPF header
#include <xspf/Xspf.h>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace PlayList {
      namespace XSPF {
        class libXSPF: virtual public Xspf::XspfReaderCallback {
          public:
            libXSPF();
            ~libXSPF();

            list <string> getTrackList() const;
            void clear();
          private:
            virtual void addTrack(Xspf::XspfTrack * track);
            virtual void setProps(Xspf::XspfProps * props);

            Xspf::XspfReader reader;
            list <string> trackList;
        };
      }
    }
  }
}

#endif
