#include "Random.hpp"

namespace SilentMedia {
  namespace Utils {
    string Random::getRandomId(const string inString) {
      boost::mt19937 rng;
      rng.seed(static_cast <unsigned> (std::time(0)));
      boost::uniform_int <> distribution(1, INT_MAX);
      boost::variate_generator <boost::mt19937&, boost::uniform_int <> > die(
          rng, distribution);

      stringstream out;
      out << inString << die();

      return out.str();
    }
  }
}
