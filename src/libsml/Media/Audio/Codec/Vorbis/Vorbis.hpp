/*
 * http://www.xiph.org/vorbis/doc/vorbisfile/index.html
 */

#ifndef _SILENTMEDIA_MEDIA_AUDIO_CODEC_VORBIS_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_CODEC_VORBIS_HPP_

// main include
#include <libsml/include.hpp>

// include boost
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

/*
 * We should include AbstractCodec, AudioProxy.
 */
#include "../AbstractCodec.hpp"
#include "../../AudioProxy.hpp"

// Vorbis include
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {
        class Vorbis: virtual public AbstractCodec {
          public:
            Vorbis();
            virtual ~Vorbis();

            virtual bool open(const string &fileId);
            virtual void close(const string &fileId);

            virtual int play(const string &fileId, bool resume = false);
            virtual void stop(const string &fileId);

            virtual float getSeek(const string &fileId);
            virtual void setSeek(const string &fileId, const double &seekVal);
          private:
            // AudioProxy object
            AudioProxy * audioProxy;

            // Vorbis File map
            map < string, OggVorbis_File > vorbisFileMap;

            // Vorbis Info map
            map < string, vorbis_info * > vorbisInfoMap;

            map < string, bool > stopMap;

            void readVorbisComment(const string &fileId);
        };
      }
    }
  }
}

#endif
