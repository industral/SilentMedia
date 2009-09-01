#include <libsml/include.hpp>
#include "Base64.hpp"

using namespace std;

int main() {
  const string
      s =
          "ADP GmbH\nAnalyse Design & Programmierung\nGesellschaft mit beschränkter Haftung";

  string encoded = base64_encode(
      reinterpret_cast < const unsigned char* > (s.c_str()), s.length());
  string decoded = base64_decode(encoded);

  cout << "encoded: " << encoded << endl;
  cout << "decoded: " << decoded << endl;

  return 0;
}
