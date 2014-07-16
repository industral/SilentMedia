#ifndef _SILENTMEDIA_MEDIA_CONTAINER_FILELOADER_HPP_
#define _SILENTMEDIA_MEDIA_CONTAINER_FILELOADER_HPP_

#include <libsml/include.hpp>
#include "../../Throw/Throw.hpp"

typedef boost::filesystem::path Path;

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Container {

      enum ContainerType {
        UNKNOW_CONTAINER, NONE_CONTAINER, RIFF_WAVE, RIFF_AVI, RIFF_UNKNOW, OGG
      };

      enum CodecType {
        UNKNOW_CODEC, NONE_CODEC, FLAC, VORBIS, THEORA, WAVPACK
      };

      class FileLoader {
        public:
          /**
           * Constructor.
           */
          FileLoader();

          /**
           * Desctuctor.
           */
          ~FileLoader();

          /**
           * Open media file.
           * param[in] fileName path to media file.
           */
          bool open(const string &fileName);

          /**
           * Close file.
           */
          void close();

          /**
           * Get file media container.
           * @return media container type.
           */
          ContainerType getContainer();

          /**
           * Get media codec.
           * @return media codec type.
           */
          CodecType getCodec();

          /**
           * Get file signature (4 first bytes).
           * @return file signature.
           */
          string getFileSignature();
        private:
          /**
           * Read segment of file.
           * @param[in] offset file offset in bytes,
           * @param[in] length size of bytes to read.
           * @return output string of read bytes.
           */
          string readFileSegment(int offset, int length);

          ifstream infile;
          string fileName;

          ContainerType container;
          CodecType codec;
      };
    }
  }
}

#endif
