#include "XSPF.hpp"

using namespace ::SilentMedia::Media;

namespace SilentMedia {
  namespace Media {
    namespace PlayList {
      namespace XSPF {

        // --------------------------------------------------------------------
        // Public methods
        // --------------------------------------------------------------------

        XSPF::XSPF() :
          libxspf(new libXSPF) {
        }

        XSPF::~XSPF() {
          delete this -> libxspf;
          this -> libxspf = NULL;
        }

        bool XSPF::openPlayList(const string &playList) {
          // Check file on any throw
          Container::FileLoader fileLoader;
          fileLoader.open(playList);
          fileLoader.close();

          XML_Char const * const baseUri = _PT("http://d-industrial.com");
          const int res = reader.parseFile(_PT(playList.c_str()), libxspf,
              baseUri);

          return (res == Xspf::XSPF_READER_SUCCESS) ? true : false;
        }

        bool XSPF::writePlayList(const string &playList,
            list <TrackInfo> playListData) {

          XML_Char const * const baseUri = _PT("http://d-industrial.com");
          Xspf::XspfIndentFormatter formatter;
          Xspf::XspfWriter * const writer = Xspf::XspfWriter::makeWriter(
              formatter, baseUri);

          for (list <TrackInfo>::iterator it = playListData.begin(); it
              != playListData.end(); ++it) {
            Xspf::XspfTrack track;

            track.giveCreator(_PT(it -> getArtist().c_str()),
                Xspf::XspfData::COPY);
            track.giveAlbum(_PT(it -> getAlbum().c_str()), Xspf::XspfData::COPY);
            track.giveTitle(_PT(it -> getTitle().c_str()), Xspf::XspfData::COPY);
            track.giveAppendLocation(_PT(it -> getTrackLocation().c_str()),
                Xspf::XspfData::COPY);

            int trackNum = it -> getTrackNumber();
            if (trackNum != -1) {
              track.setTrackNum(it -> getTrackNumber());
            }

            writer -> addTrack(track);
          }

          int const res = writer -> writeFile(playList.c_str());

          return (res == Xspf::XSPF_WRITER_SUCCESS) ? true : false;
        }

        bool XSPF::close() {
          this -> libxspf -> clear();
          return true; // TODO: close file
        }

        list <string> XSPF::getPlayList() const {
          return (this -> libxspf -> getTrackList());
        }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      }
    }
  }
}
