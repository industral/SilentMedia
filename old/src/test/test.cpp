#include <iostream>
#include <SML/all.hpp>

using namespace std;

int main() {
  cout << "Hello, world!" << endl;

  SilentMedia::Audio * audio = SilentMedia::Audio::Instance();

  audio -> init(_Game, _DSP, _ALSA);

  return 0;
}
