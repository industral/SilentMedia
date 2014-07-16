#ifndef SILENTMEDIA_UTIL_RANDOM_RANDOM_HPP_
#define SILENTMEDIA_UTIL_RANDOM_RANDOM_HPP_

#include <libsml/include.hpp>

using namespace std;

namespace SilentMedia {
  namespace Utils {
    class Random {
      public:
        static string getRandomId(const string inString);
      private:
    };
  }
}

#endif
