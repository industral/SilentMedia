#include "WavPack.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {
        WavPack::WavPack() {

          // create instance for AudioProxy
          this -> audioProxy = new AudioProxy();
        }

        WavPack::~WavPack() {
          // release resources
          delete this -> audioProxy;
          this -> audioProxy = NULL;
        }

        bool WavPack::open(const string &fileId) {
          // get fileName
          string fileName = this -> audioProxy -> getFileNameByFileId(fileId);

          if (!boost::filesystem::exists(fileName)) {
            return false;
          }

          char * error = NULL;

          this -> wavPackContextMap[fileId] = WavpackOpenFileInput(
              fileName.c_str(), error, OPEN_WVC, 0);

          if (this -> wavPackContextMap[fileId] == NULL) {
            cerr << "Error in error WavpackOpenFileInput(): " << error << endl;
          }

          long totalTime = WavpackGetNumSamples(
              this -> wavPackContextMap[fileId]) / WavpackGetSampleRate(
              this -> wavPackContextMap[fileId]);

          // update audio information
          this -> audioProxy -> setAudioParams(fileId, fileName,
              WavpackGetFileSize(this -> wavPackContextMap[fileId]), totalTime,
              WavpackGetNumChannels(this -> wavPackContextMap[fileId]),
              WavpackGetSampleRate(this -> wavPackContextMap[fileId]),
              WavpackGetAverageBitrate(this -> wavPackContextMap[fileId], 0),
              WavpackGetBitsPerSample(this -> wavPackContextMap[fileId]));

          //    this -> readVorbisComment();

          return true;
        }

        int WavPack::play(const string &fileId, bool resume) {
          // set params if it in first time
          if (!resume) {
            // now set parameters to sound system
            this -> audioProxy -> setSoundSystemParams(fileId);
          }

          const int bufSize = 4096;

          int32_t *buffer = new int32_t[bufSize];
          int16_t *outputBuf = new int16_t[bufSize];

          while ( /*int samplesUnpacked =*/WavpackUnpackSamples(
              this -> wavPackContextMap[fileId], buffer, 1024)) {

            if (this -> stopMap[fileId]) {
              this -> stopMap[fileId] = false;
//              this -> close(fileId);
              return 0;
            }

            for (int i = 0; i < bufSize; ++i) {
              outputBuf[i] = buffer[i];
            }
            this -> audioProxy -> write(outputBuf, bufSize);
          }

          // release resource
          delete[] buffer;
          delete[] outputBuf;

          this -> close(fileId);
          return 0;
        }

        void WavPack::stop(const string &fileId) {
          this -> stopMap[fileId] = true;
        }

        void WavPack::setSeek(const string &fileId, const double &seekVal) {
          uint32_t sample = ((WavpackGetNumSamples(
              this -> wavPackContextMap[fileId])) * (seekVal / 100));

          if (WavpackSeekSample(this -> wavPackContextMap[fileId], sample)
              == false) {
            cerr << "Error in WavpackSeekSample()" << endl;
            this -> close(fileId);
          }
        }

        float WavPack::getSeek(const string &fileId) {
          if (WavpackGetSampleIndex(this -> wavPackContextMap[fileId]) == 0) {
            return 0;
          } else {
            return (100 / (static_cast < double > (WavpackGetNumSamples(
                this -> wavPackContextMap[fileId])) / WavpackGetSampleIndex(
                this -> wavPackContextMap[fileId])));
          }
        }

        void WavPack::close(const string &fileId) {
          WavpackCloseFile(this -> wavPackContextMap[fileId]);
        }

      }
    }
  }
}
