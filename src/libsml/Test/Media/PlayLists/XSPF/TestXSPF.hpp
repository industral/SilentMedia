#ifndef _SILENTMEDIA_TEST_MEDIA_PLAYLIST_XSPF_TESTXSPF_HPP_
#define _SILENTMEDIA_TEST_MEDIA_PLAYLIST_XSPF_TESTXSPF_HPP_

#include <libsml/all.hpp>

#include <gtest/gtest.h>

#include <Media/PlayList/XSPF/XSPF.hpp>

using namespace std;
using namespace ::SilentMedia::Media::PlayList::XSPF;

namespace SilentMedia {
  namespace Test {
    namespace Media {
      namespace PlayList {
        namespace XSPF {
          class TestXSPF: public ::testing::Test {
            protected:
            TestXSPF();
              virtual ~TestXSPF();

              virtual void SetUp();
              virtual void TearDown();

              ::SilentMedia::Media::PlayList::XSPF::XSPF * xspf;
          };
        }
      }
    }
  }
}

#endif
