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

#include "TestFileLoader.hpp"

using namespace std;
using namespace SilentMedia;
using namespace SilentMedia::Media;

namespace SilentMedia {
  namespace Test {
    namespace Media {
      namespace Container {
        namespace FileLoader {

          TestFileLoader::TestFileLoader() {
          }

          TestFileLoader::~TestFileLoader() {
          }

          void TestFileLoader::SetUp() {
            this -> loader = new ::SilentMedia::Media::Container::FileLoader();
          }

          void TestFileLoader::TearDown() {
            delete this -> loader;
            this -> loader = NULL;
          }

          /**
           * Check to open non-existing file.
           */
          TEST_F(TestFileLoader, FileNotFound)
          {
            const string fakeFilePath = "/etc/bla-bla-bla";
            EXPECT_THROW(loader -> open(fakeFilePath), ::SilentMedia::Throw::FileNotFound);

            try {
              loader -> open(fakeFilePath);
            } catch (::SilentMedia::Throw::FileNotFound theException) {
              string expectedOutputMessage = "Exception: File not found: " + fakeFilePath;
              EXPECT_EQ(expectedOutputMessage, theException.getMessage());
            }
          }

          /**
           * Check to open directory instead of file.
           */
          TEST_F(TestFileLoader, NotRegularFile) {
            const string testFolder = "/etc";
            EXPECT_THROW(loader -> open(testFolder), ::SilentMedia::Throw::NotRegularFile);
          }

          /**
           * Check to open file with wrong permission.
           */
          TEST_F(TestFileLoader, ErrorOpenFile) {
            const string testFile = "/etc/shadow";
            EXPECT_THROW(loader -> open(testFile), ::SilentMedia::Throw::ErrorOpenFile);
          }

          /**
           * Check to open regular file.
           */
          TEST_F(TestFileLoader, TestFileOpen) {
            const string testFile = "/etc/passwd";
            EXPECT_NO_THROW(loader -> open(testFile));
          }

          TEST_F(TestFileLoader, TestAudioContainerAndCodec) {
            using namespace ::SilentMedia::Media::Container;
            const string testFile = "/data/music/test/file.ogg";

            const ContainerType vorbisContainer = OGG;
            const CodecType vorbisCodec = VORBIS;

            ASSERT_NO_THROW(loader -> open(testFile));

            EXPECT_EQ(vorbisContainer, loader -> getContainer());
            EXPECT_EQ(vorbisCodec, loader -> getCodec());
          }

          TEST_F(TestFileLoader, TestCloseFile) {
            EXPECT_NO_THROW(loader -> close());
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
