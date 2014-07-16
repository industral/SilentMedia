#include <libsml/all.hpp>
#include <libsml/Media/Audio/SoundSystem/ALSA/Mixer/Mixer.hpp>

using namespace SilentMedia::Media::Audio::SoundSystem::ALSA::Mixer;

int main() {
  Mixer *mixer = new Mixer();
  mixer -> open();

  delete mixer;
  mixer = NULL;

  return 0;
}
