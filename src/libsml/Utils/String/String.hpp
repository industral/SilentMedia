#ifndef SILENTMEDIA_UTIL_STRING_STRING_HPP_
#define SILENTMEDIA_UTIL_STRING_STRING_HPP_

#include <libsml/include.hpp>

using namespace std;

namespace SilentMedia {
  namespace Utils {
    class String {
      public:
        static string toXML(const string &inString);
        static string fromXML(const string &inString);
      private:
    };
  }
}

#endif
