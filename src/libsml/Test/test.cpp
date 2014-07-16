#include <libsml/all.hpp>

using namespace std;
using namespace SilentMedia;
using namespace SilentMedia::Media;

int main() {
  Audio::Audio * audio = new Audio::Audio();
  audio -> init("ALSA", ""); // init Audio system

  //  Audio::Audio * audio2 = new Audio::Audio();
  //  audio2 -> init(); // init Audio system

  string fileId = "file1";

  try {
    audio -> open("src/test/music/file.ogg", fileId);
  }
  catch (Throw::File e) {
    cout << e.getMessage() << endl;
  }

  //  if (audio -> open("src/test/music/file.ogg", fileId)) {
  //
  //    //  audio -> getInfo("file1");
  //
  //    audio -> play("file1");
  //    //  audio -> pause("file1");
  //    //  audio -> write("file1");
  //    //  audio -> stop("file1");
  //    //  audio -> close("file1");
  //  }

  delete audio;
  audio = NULL;

  //  string fileId2 = "file2";
  //
  //  if (audio2 -> open("src/test/music/file.ogg", fileId2)) {
  //    audio2 -> play("file2");
  //  }

  //  audio -> finish();


  //  delete audio2;
  //  audio2 = NULL;

  return 0;
}
