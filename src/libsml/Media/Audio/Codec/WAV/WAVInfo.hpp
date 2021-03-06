#ifndef _SILENTMEDIA_MEDIA_AUDIO_CODEC_WAVINFO_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_CODEC_WAVINFO_HPP_

/**
 * @see http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
 * @see http://technology.niagarac.on.ca/courses/ctec1631/WavFileFormat.html
 * @see http://www.sonicspot.com/guide/wavefiles.html
 */
namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {
        class WAVInfo {
          public:
            //TODO: write doxygen doc
            string getChunkID() {
              string dest;
              for (int i = 0; i < 4; ++i) {
                dest += ChunkID[i];
              }
              return dest;
            }

            int getChunkSize() {
              return this -> ChunkSize;
            }

            string getFormat() {
              string dest;
              for (int i = 0; i < 4; ++i) {
                dest += Format[i];
              }
              return dest;
            }

            string getSubchunk1ID() {
              string dest;
              for (int i = 0; i < 4; ++i) {
                dest += Subchunk1ID[i];
              }
              return dest;
            }

            int getSubchunk1Size() {
              return this -> Subchunk1Size;
            }

            int getAudioFormat() {
              return this -> AudioFormat;
            }

            int getNumChannels() {
              return this -> NumChannels;
            }

            int getSampleRate() {
              return this -> SampleRate;
            }

            int getByteRate() {
              return this -> ByteRate;
            }

            int getBlockAlign() {
              return this -> BlockAlign;
            }

            int getBitsPerSample() {
              return this -> BitsPerSample;
            }

            string getSubchunk2ID() {
              string dest;
              for (int i = 0; i < 4; ++i) {
                dest += Subchunk2ID[i];
              }
              return dest;
            }

            int getSubchunk2Size() {
              return this -> Subchunk2Size;
            }
          private:
            /*
             bit -          0 or 1
             byte -         8 bit
             kbit - 10e3
             */
            /*  RIFF  */

            /*  WAVE  */
            //  Contains the letters "RIFF" in ASCII form
            //  (0x52494646 big-endian form)
            char ChunkID[4];
            int32_t ChunkSize; // size of wav file at all
            //  Contains the letters "WAVE"
            //  (0x57415645 big-endian form)
            char Format[4];

            /*  fmt  */
            char Subchunk1ID[4];
            int32_t Subchunk1Size;
            //  PCM = 1 (i.e. Linear quantization)
            //  Values other than 1 indicate some form of compression.
            int16_t AudioFormat;
            int16_t NumChannels; // Mono = 1, Stereo = 2, etc.
            int32_t SampleRate; // 8000, 44100, etc.
            int32_t ByteRate; // SampleRate * NumChannels * BitsPerSample (kbit/s)
            //  == NumChannels * BitsPerSample/8
            //  The number of bytes for one sample including all channels
            int16_t BlockAlign;
            int16_t BitsPerSample; // 8 bits = 8, 16 bits = 16, etc.

            /*  data  */
            char Subchunk2ID[4]; // Contains the letters "data"
            int32_t Subchunk2Size; // NumSamples * NumChannels * BitsPerSample/8
        };
      }
    }
  }
}

#endif
