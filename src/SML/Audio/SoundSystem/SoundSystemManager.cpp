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

#include "SoundSystemManager.hpp"

#include <SML/Audio/SoundSystem/AbstractSoundSystem.hpp>
#include <SML/Audio/SoundSystem/OSS/DSP/OSS_DSP.hpp>
#include <SML/Audio/SoundSystem/ALSA/DSP/ALSA_DSP.hpp>

SilentMedia::SoundSystemManager * SilentMedia::SoundSystemManager ::_ssystem = NULL;

SilentMedia::SoundSystemManager::SoundSystemManager ( void ) : ssystem ( NULL ), dsp ( NULL ) {
//       std::cout << "SoundSystemManager Constructor.. " << std::endl;
//    this -> dsp = SilentMedia::SoundSystem::OSS::DSP::Instance();
}

SilentMedia::SoundSystemManager::~SoundSystemManager ( void ) {
//    std::cout << "SoundSystemManager Destructor.. " << std::endl;
   for ( int i = 0; i <= 128; ++i ) {
      delete this -> dspObjs [ i ];
   }
}

SilentMedia::SoundSystemManager * SilentMedia::SoundSystemManager::Instance ( void ) {
   if ( _ssystem == NULL ) {
      _ssystem = new SoundSystemManager();
   }
   return ( _ssystem );
}

// bool SilentMedia::SoundSystemManager::initMultiPlay ( void ) {
//    for ( int i = 0; i <= 128; ++i ) {
//       this -> dspObjs [ i ] = getNeedObj();
//       this -> dspObjs [ i ] -> init ( this -> dspDev );
// //       this -> dspObjs [ i ] -> setParams ( channels, sampleRate );
//       this -> dspList . push_back ( i );
//    }
//    return true;
// }

bool SilentMedia::SoundSystemManager::init ( AudioComponents component, SoundSystemType soundSystem, std::string dspDev, std::string mixerDev ) {

   this -> selectedSoundSystem = soundSystem;
   this -> dspDev = dspDev;

#if 0
   switch ( selectedSoundSystem ) {
      case _OSS:
         switch ( component ) {
            case _DSP:
               this -> dsp = SilentMedia::SoundSystem::OSS::DSP::Instance();
               this -> dsp -> init ( dspDev );
         case _MIXER:
            this -> mixer = SilentMedia::OSS::DSP::Instance();
            this -> mixer -> init ( this -> selectedSoundSystemManager, mixerDev );
         }
      case _ALSA:
         switch ( component ) {
            case _DSP:
               this -> dsp = SilentMedia::SoundSystem::ALSA::DSP::Instance();
               this -> dsp -> init ( "default" );
         }
   }
#endif
   for ( int i = 0; i <= 128; ++i ) {
      this -> dspObjs [ i ] = getNeedObj();
      this -> dspObjs [ i ] -> init ( this -> dspDev );
//       this -> dspObjs [ i ] -> setParams ( channels, sampleRate );
      this -> dspList . push_back ( i );

   }
   return true;
}

bool SilentMedia::SoundSystemManager::play ( const void * buf, std::string id ) {
   this -> dspObjs [  this -> dspMap [ id ] ] -> play ( buf );
   return true;
}

SilentMedia::AbstractSoundSystem * SilentMedia::SoundSystemManager::getNeedObj ( void ) {
   switch ( this -> selectedSoundSystem ) {
      case _OSS:
         return ( new SilentMedia::SoundSystem::OSS::DSP() );
      case _ALSA:
         return ( new SilentMedia::SoundSystem::ALSA::DSP() );
   }
}

void SilentMedia::SoundSystemManager::allocateQuery ( std::string id ) {
   this -> id = id;
   this -> dspMap [ id ] = this -> dspList . back();
   this -> dspList . pop_back();
}

void SilentMedia::SoundSystemManager::deAllocateQuery ( std::string id ) {
   this -> dspObjs [  this -> dspMap [ id ] ] -> stop();
   this -> dspList . push_back ( this -> dspMap [ id ] );
}

bool SilentMedia::SoundSystemManager::setParams ( unsigned short int channels, unsigned int sampleRate ) {
   this -> dspObjs [ this -> dspMap [ this -> id ] ] -> setParams ( channels, sampleRate );
   this -> dspObjs [  this -> dspMap [ id ] ] -> start();
}
