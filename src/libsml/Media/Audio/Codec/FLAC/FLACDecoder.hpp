#ifndef _SILENTMEDIA_MEDIA_AUDIO_CODEC_FLACDECODER_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_CODEC_FLACDECODER_HPP_

#include "FLAC.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {
        class FLAC;

        /**
         * FLAC Decoder class. Inheritance from ::FLAC::Decoder::File.
         * @see http://flac.sourceforge.net/api/classFLAC_1_1Decoder_1_1File.html
         */
        class FLACDecoder: public ::FLAC::Decoder::File {
          public:
            FLACDecoder();

            virtual FLAC__StreamDecoderWriteStatus write_callback(
                const FLAC__Frame * frame, const FLAC__int32 * const buf[]);

            virtual void
                error_callback(::FLAC__StreamDecoderErrorStatus status);

            void setFileId(const string &fileId);

          private:
            FLAC * flacObj;
            string fileId;
        };
      }
    }
  }
}

#endif
