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

#include "TestXSPF.hpp"

using namespace std;

namespace SilentMedia {
  namespace Test {
    namespace Media {
      namespace PlayList {
        namespace XSPF {

          TestXSPF::TestXSPF() {
          }

          TestXSPF::~TestXSPF() {
          }

          void TestXSPF::SetUp() {
            this -> xspf = new ::SilentMedia::Media::PlayList::XSPF::XSPF();
          }

          void TestXSPF::TearDown() {
            delete this -> xspf;
            this -> xspf = NULL;
          }

          TEST_F(TestXSPF, TestOpenFile)
          {
            const string fakeFilePath = "__nofile__";
            const string filePath =
                "../src/libsml/Test/Media/PlayLists/XSPF/sample.xspf";

            EXPECT_THROW(this -> xspf -> openPlayList(fakeFilePath), ::SilentMedia::Throw::FileNotFound);
            EXPECT_NO_THROW(this -> xspf -> openPlayList(filePath));
          }

          TEST_F(TestXSPF, TestWriteToPlayList) {
            using namespace ::SilentMedia::Media::PlayList;

            const string prefix = "file://";

            TrackInfo trackInfo1;
            TrackInfo trackInfo2;
            TrackInfo trackInfo3;

            list <TrackInfo> playListData;

            const string playList = "testWrite.xspf";

            const string artist1 = "Rammstein";
            const string album1 = "Mutter";
            const string title1 = "Ich Will";
            const int trackNum1 = 4;

            const string trackLocation1 = "/data/music/Rammstein/Rammstein - Mutter/04 Ich Will.flac";
            const string escapeTraclLocation1 = ::SilentMedia::Utils::String::toXML(trackLocation1);
            const string finalTrackLocation1 = prefix + escapeTraclLocation1;

//            trackInfo1.setArtist(artist1);
//            trackInfo1.setAlbum(album1);
//            trackInfo1.setTitle(title1);
//            trackInfo1.setTrackNumber(trackNum1);
            trackInfo1.setTrackLocation(finalTrackLocation1);

            const string artist2 = "Korn";
            const string album2 = "Evolution";
            const string title2 = "Evolution";
            const int trackNum2 = 4;
            const string trackLocation2 = "/data/music/Korn/(2007) Untitled/04. Evolution.flac";
            const string escapeTraclLocation2 = ::SilentMedia::Utils::String::toXML(trackLocation2);
            const string finalTrackLocation2 = prefix + escapeTraclLocation2;

//            trackInfo2.setArtist(artist2);
//            trackInfo2.setAlbum(album2);
//            trackInfo2.setTitle(title2);
//            trackInfo2.setTrackNumber(trackNum2);
            trackInfo2.setTrackLocation(finalTrackLocation2);

            const string artist3 = "Silent Hill";
            const string album3 = "Silent Hill3";
            const string title3 = "Player";
            const int trackNum3 = 15;

//            trackInfo3.setArtist(artist3);
//            trackInfo3.setAlbum(album3);
//            trackInfo3.setTitle(title3);
//            trackInfo3.setTrackNumber(trackNum3);

            playListData.push_back(trackInfo1);
            playListData.push_back(trackInfo2);
            playListData.push_back(trackInfo3);

            this -> xspf -> writePlayList(playList, playListData);
          }

        }
      }
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
