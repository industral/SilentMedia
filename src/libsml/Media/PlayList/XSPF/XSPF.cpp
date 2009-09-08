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
