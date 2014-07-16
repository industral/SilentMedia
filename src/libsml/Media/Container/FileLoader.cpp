#include "FileLoader.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Container {

      // --------------------------------------------------------------------
      // Public methods
      // --------------------------------------------------------------------

      FileLoader::FileLoader() {
      }

      FileLoader::~FileLoader() {
      }

      bool FileLoader::open(const string &fileName) {
        this -> fileName = fileName;

        Path path(fileName);

        if (!boost::filesystem::exists(path)) {
          throw SilentMedia::Throw::FileNotFound(fileName);
        }
        if (!boost::filesystem::is_regular_file(path)) {
          throw SilentMedia::Throw::NotRegularFile(fileName);
        }

        this -> infile.open(fileName.c_str(), ios::binary);

        if (!this -> infile.is_open()) {
          throw SilentMedia::Throw::ErrorOpenFile(fileName);
        }

        return true;
      }

      void FileLoader::close() {
        this -> infile.close();

        if (this -> infile.is_open()) {
          throw SilentMedia::Throw::ErrorCloseFile(this -> fileName);
        }
      }

      ContainerType FileLoader::getContainer() {
        // offset = 0, length = 4
        string signature = this -> readFileSegment(0, 4);
        if (!signature.compare("OggS")) {
          this -> container = OGG;
        } else if (!signature.compare("RIFF")) {
          // RIFF have WAVE and AVI, so clarify it
          string containerDetail = this -> readFileSegment(8, 4);
          if (!containerDetail.compare("WAVE")) {
            this -> container = RIFF_WAVE;
          } else if (!containerDetail.compare("AVI")) {
            this -> container = RIFF_AVI;
          } else {
            this -> container = RIFF_UNKNOW;
          }
        } else {
          this -> container = UNKNOW_CONTAINER;
        }
        return this -> container;
      }

      //TODO: Need to return 2 codecs, audio & video
      CodecType FileLoader::getCodec() {
        /* OGG */
        if (this -> container == OGG) {
          // offset = 29, length = 6
          string mediaCodec = this -> readFileSegment(29, 6);
          if (!mediaCodec.compare("vorbis")) {
            this -> codec = VORBIS;
          } else if (!mediaCodec.compare("theora")) {
            this -> codec = THEORA;
          }
        } else {
          this -> codec = UNKNOW_CODEC;
        }
        return this -> codec;
      }

      string FileLoader::getFileSignature() {
        return this -> readFileSegment(0, 4);
      }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      string FileLoader::readFileSegment(int offset, int length) {
        // need to be every time set to global start point
        this -> infile.seekg(offset, ios_base::beg);

        char buf[length + 1];
        this -> infile.read(buf, length);
        buf[length] = 0;

        return buf;
      }
    }
  }
}
