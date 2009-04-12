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

#include "DecodedData.hpp"

#include <SML/Audio/SoundSystem/SoundSystemManager.hpp>
#include <SML/Audio/Audio.hpp>

SilentMedia::DecodedData * SilentMedia::DecodedData::_ddata = NULL;

SilentMedia::DecodedData::DecodedData ( void ) : ssystem ( NULL ), playCheck ( false ) {
   this -> ssystem = SoundSystemManager::Instance();

//    this -> audio = Audio::Instance();
}

SilentMedia::DecodedData::~DecodedData ( void ) {
//    std::cout << "DecodedData Destructor.. " << std::endl;
}

SilentMedia::DecodedData * SilentMedia::DecodedData::Instance ( void ) {
   if ( _ddata == NULL ) {
         _ddata = new DecodedData();
   }
   return ( _ddata );
}

void SilentMedia::DecodedData::setMainParam (
      std::string fileName,
      long int    fileSize,
      double      totalTime,
      int         channels,
      int         sampleRate,
      double      bitRate,
      int         bitsPerSample ) {
         this -> fileName = fileName;
         this -> fileSize = fileSize;
         this -> totalTime = totalTime;
         this -> channels = channels;
         this -> sampleRate = sampleRate;
         this -> bitRate = bitRate;
         this -> bitsPerSample = bitsPerSample;
         
//          this -> archive
}

bool SilentMedia::DecodedData::play ( const void * buf, std::string id ) {
   this -> ssystem -> play ( buf, id );
   return true;
}

void SilentMedia::DecodedData::begin ( std::string id ) {
   this -> ssystem -> allocateQuery ( id );
   this -> ssystem -> setParams ( this -> channels, this -> sampleRate );
}

void SilentMedia::DecodedData::end ( std::string id ) {
   this -> audio = Audio::Instance();

   this -> ssystem -> deAllocateQuery ( id );
   this -> audio -> destroyObj ( id );
}

// int SilentMedia::DecodedData::getDSPDev ( void ) const {
//    return ( this -> ssystem -> getDSPDev() );
// }

void SilentMedia::DecodedData::clean ( void ) {
   this -> picData . clear();
   this -> vorbisComm . clear();
}

// void SilentMedia::Audio::setCover ( std::string coverData ) {
// //    if ( this -> ext == "flac" ) {
// //       objs [ "flac" ] -> setPicture ( coverData );
// //       FlacDecode flacdec;
// //       flacdec . setPicture ( coverData );
// //    }
// }
