#include "libXSPF.hpp"

namespace SilentMedia {
  namespace Media {
    namespace PlayList {
      namespace XSPF {

        libXSPF::libXSPF() {
        }

        libXSPF::~libXSPF() {
        }

        void libXSPF::addTrack(Xspf::XspfTrack * track) {
          const int trackCount = track -> getLocationCount();

          for (int i = 0; i < trackCount; ++i) {
            this -> trackList.push_back((string) track -> getLocation(i));
          }
        }

        list <string> libXSPF::getTrackList() const {
          return this -> trackList;
        }

        void libXSPF::setProps(Xspf::XspfProps * props) {
          //          cout << "In setProper: " << endl;
        }

        void libXSPF::clear() {
          this -> trackList.clear();
        }

      }
    }
  }
}
