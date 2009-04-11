/***************************************************************************
 *   Copyright (C) 2008 by Alex J. Ivasyuv                                 *
 *   alex@siegerstein.org.ua                                               *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef __SS_SILENTMEDIA_WAV_HPP_
#define __SS_SILENTMEDIA_WAV_HPP_

#include <SML/slib.hpp>
#include "../AbstractCodec.hpp"

namespace SilentMedia {
   class DecodedData;
   namespace Codec {
      class WAV : virtual public AbstractCodec {
         public:
            WAV ( void );
            virtual ~WAV ( void );

         // virtual
            virtual bool init ( std::string inputfile, std::string idObj );
            virtual bool play ( std::string id );
            virtual unsigned long int getSeek ( void ) const;
            virtual void setSeek ( double val );
            virtual void flush ( void );
         private:
            class WavInfo {
               public:
// 			WavInfo ( void );
//                ~WavInfo ( void ) { std::cout << "WavInfo Destructor.. " << std::endl; }

                  std::string getChunkID ( void ) {
                     std::string dest;
                     for ( unsigned short int i = 0; i != 4; ++i ) {
                        dest += ChunkID [ i ];
                     }
                     return dest;
                  }

                  int getChunkSize ( void ) { return ( this -> ChunkSize ); }

                  std::string getFormat ( void ) {
                     std::string dest;
                     for ( unsigned short int i = 0; i != 4; ++i ) {
                        dest += Format [ i ];
                     }
                     return dest;
                  }

                  std::string getSubchunk1ID ( void ) {
                     std::string dest;
                     for ( unsigned short int i = 0; i != 4; ++i ) {
                        dest += Subchunk1ID [ i ];
                     }
                     return dest;
                  }

                  int getSubchunk1Size ( void ) { return ( this -> Subchunk1Size ); }
                  int getAudioFormat ( void ) { return ( this -> AudioFormat ); }
                  int getNumChannels ( void ) { return ( this -> NumChannels ); }
                  int getSampleRate ( void ) { return ( this -> SampleRate ); }
                  int getByteRate ( void ) { return ( this -> ByteRate ); }
                  int getBlockAlign ( void ) { return ( this -> BlockAlign ); }
                  int getBitsPerSample ( void ) { return ( this -> BitsPerSample ); }

                  std::string getSubchunk2ID ( void ) {
                     std::string dest;
                     for ( unsigned short int i = 0; i != 4; ++i ) {
                        dest += Subchunk2ID [ i ];
                     }
                     return dest;
                  }

                  int getSubchunk2Size ( void ) { return ( this -> Subchunk2Size ); }
               private:
         /*
                  bit - бит         0 или 1
                  byte - байт       8 бит
                  kbit - 10e3
         */
// 	== RIFF ==
	
// 	WAVE
// 	Contains the letters "RIFF" in ASCII form
// 	(0x52494646 big-endian form)
                  char ChunkID [ 4 ];
                  int32_t ChunkSize; // размер "wav" файла целиком
// 	Contains the letters "WAVE"
// 	(0x57415645 big-endian form)
                  char Format [ 4 ];
		
// 	fmt
                  char Subchunk1ID [ 4 ];
                  int32_t Subchunk1Size;
// 	PCM = 1 (i.e. Linear quantization)
// 	Values other than 1 indicate some form of compression.
                  int16_t AudioFormat;
                  int16_t NumChannels; // Mono = 1, Stereo = 2, etc.
                  int32_t SampleRate; // 8000, 44100, etc.
                  int32_t ByteRate; // SampleRate * NumChannels * BitsPerSample (kbit/s)
// 	== NumChannels * BitsPerSample/8
// 	The number of bytes for one sample including all channels
                  int16_t BlockAlign;
                  int16_t BitsPerSample; // 8 bits = 8, 16 bits = 16, etc.

// 	data
                  char Subchunk2ID [ 4 ];
                  int32_t Subchunk2Size; // размер аудио-данных ( pcm )
            };
            DecodedData * ddata;
            WavInfo * wavinfo;

            int input_fd;  // файловый дексриптор воспроизводящегося файла
//             int dspDev;
            FILE * pFile; // файловый дискриптор
            std::string inputFile;
            std::string idObj;

            unsigned long int offsetPos; // текущее считаное количество байт
            double fileSize; // должно быть double чтобы не делать static_cast
         // Дополнительная информация которой нету в класе Wav
            double totalTime;
            long int totalSamples;
            int bitRate;

         // стражи
            bool playCheck;
            bool seekCheck;
      };
   }
}

#endif
