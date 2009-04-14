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

#include "WavPack.hpp"
#include <SML/Audio/Codec/DecodedData.hpp>

SilentMedia::Codec::WavPack::WavPack ( void ) : wpc ( NULL ), pwpc ( NULL ), bufSize ( 4096 ), seekCheck ( false ),
      playCheck ( false ), fileSize ( -1 ), totalTime ( -1 )
{
   this -> ddata = DecodedData::Instance();
}

SilentMedia::Codec::WavPack::~WavPack ( void )
{ }

bool SilentMedia::Codec::WavPack::init ( std::string inputFile ) {
   this -> dspDev = this -> ddata -> getDSPDev();
   std::cout << "WavPack: in init()" << std::endl;
   this -> fileName = inputFile;

   char * error = NULL;
   this -> wpc = WavpackOpenFileInput ( this -> fileName . c_str(), error, OPEN_WVC, 0 );
   if ( this -> wpc == NULL ) {
      std::cout << "Error in error WavpackOpenFileInput(): " << error << std::endl;
   }

   this -> fileSize = WavpackGetFileSize ( this -> wpc );
   this -> totalTime = WavpackGetNumSamples ( this -> wpc ) / WavpackGetSampleRate ( this -> wpc );

   this -> ddata -> setAudioParams ( this -> fileSize, this -> totalTime, WavpackGetNumChannels ( this -> wpc ), WavpackGetSampleRate ( this -> wpc ),
                             WavpackGetAverageBitrate ( this -> wpc, 0 ), WavpackGetBitsPerSample ( this -> wpc ) );
//    this -> readVorbisComment();

   return 0;
}

void SilentMedia::Codec::WavPack::play ( void ) {
   std::cout << "WavPack: in play()" << std::endl;
   this -> playCheck = true;

   this -> buffer = new int32_t [ this -> bufSize ] ;
   this -> outputBuf = new int16_t [ this -> bufSize ];

   if ( this -> seekCheck == false ) {
      char * error = NULL;
      this -> pwpc = WavpackOpenFileInput ( this -> fileName . c_str(), error, OPEN_WVC, 0 );
      if ( this -> pwpc == NULL ) {
         std::cout << "Error in error WavpackOpenFileInput(): " << error << std::endl;
      }
   }

   this -> ddata -> setParam();
   while ( /*int samplesUnpacked =*/ WavpackUnpackSamples ( this -> pwpc, this -> buffer, 1024 ) ) {
      for ( unsigned int i = 0; i < this -> bufSize; ++i ) {
         this -> outputBuf [ i ] = this -> buffer [ i ];
      }
      write ( this -> dspDev, this -> outputBuf, this -> bufSize );
   }
   this -> releaseBuff();
}

void SilentMedia::Codec::WavPack::releaseBuff ( void ) {
   delete[] this -> buffer;
   delete[] this -> outputBuf;
}

void SilentMedia::Codec::WavPack::setSeekPos ( double seekPos ) {
   this -> seekCheck = true;

   if ( WavpackSeekSample ( this -> pwpc, ( ( WavpackGetNumSamples ( this -> pwpc ) ) * ( seekPos / 100 ) ) ) == false ) {
      std::cout << "Error in WavpackSeekSample()" << std::endl;
   }
}

unsigned long int SilentMedia::Codec::WavPack::getCurrSeekPos ( void ) const {
   if ( WavpackGetNumSamples ( this -> pwpc ) == 0 || ( WavpackGetSampleIndex ( this -> pwpc ) ) == 0 ) {
      return 0;
   } else {
      return ( 100 / ( static_cast < double > ( WavpackGetNumSamples ( this -> pwpc ) ) / WavpackGetSampleIndex ( this -> pwpc ) ) );

   }
}

void SilentMedia::Codec::WavPack::flush ( void ) {
   std::cout << "WavPack: in flush()" << std::endl;
//    if ( this -> wpc != NULL ) {
//       WavpackCloseFile ( this -> wpc );
//    }
   if ( this -> pwpc != NULL ) {
      WavpackCloseFile ( this -> pwpc );
   }

   if ( this -> playCheck == true ) {
      this -> releaseBuff();
   }
   this -> seekCheck = false;
}

void SilentMedia::Codec::WavPack::finish ( void ) {
   std::cout << "WavPack: in finish()" << std::endl;
   WavpackCloseFile ( this -> wpc );
}
