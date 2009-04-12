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

#include "/usr/lib/oss/include/sys/soundcard.h"
#include "OSS_DSP.hpp"

SilentMedia::SoundSystem::OSS::DSP * SilentMedia::SoundSystem::OSS::DSP ::_dsp = NULL;

SilentMedia::SoundSystem::OSS::DSP::DSP ( void ) : dspDev ( -1 ), input_fd ( -1 )
{ }

SilentMedia::SoundSystem::OSS::DSP::~DSP ( void ) {
//    std::cout << "DSP Destructor.. " << std::endl;
   if ( close ( this -> dspDev ) != 0 ) {
      perror ( "dsp_fd" );
   }
}

SilentMedia::AbstractSoundSystem * SilentMedia::SoundSystem::OSS::DSP::Instance ( void ) {
   if ( _dsp == NULL ) {
         _dsp = new DSP();
   }
   return ( _dsp );
}

bool SilentMedia::SoundSystem::OSS::DSP::init ( std::string dspDev_ ) {
   std::cout << "DEV: " << dspDev_ << std::endl;
   this -> dspDev = open ( dspDev_ . c_str(), O_WRONLY );
   if ( this -> dspDev == -1 ) {
      return ( _debug ( dspDev_ ) );
   }
   
//    /* Make the file descriptor use blocking writes with fcntl() */
// 
//    long flags;
//    flags = fcntl(this -> dspDev, F_GETFL);
//    flags &= ~O_ASYNC;
//    if ( fcntl(this -> dspDev, F_SETFL, flags) < 0 ) {
// //       SDL_SetError("Couldn't set audio blocking mode");
// //       DSP_CloseAudio(this);
//       return(-1);
//    }
   return true;
}

// bool SilentMedia::SoundSystem::OSS::DSP::play ( void ) {
// //       write ( dspDev, buf, actlen );
//       return true;
// }

bool SilentMedia::SoundSystem::OSS::DSP::play ( const void * buf ) {
      write ( dspDev, buf, 4096 );
   return true;
}

// int SilentMedia::SoundSystem::OSS::DSP::getDSPDev ( void ) const {
//    return ( this -> dspDev );
// }

// void SilentMedia::SoundSystem::OSS::DSP::suspend ( void ) {
//    // Нужно подвешивать устройство, решаются много интересных проблем :)
//    ioctl ( this -> dspDev, SNDCTL_DSP_HALT, NULL );
// }

bool SilentMedia::SoundSystem::OSS::DSP::setParams ( int channels, int sampleRate ) {
   int FORMAT = AFMT_S16_NE;
   // NOTE: _Не_ устанавливаем channels как short!
   int NCHANS = channels;
   int RATE = sampleRate;

//    int frag = 0x00020000;
//    std::cout << frag << std::endl;

//    std::cout << channels << std::endl;
//    std::cout << sampleRate << std::endl;
   
   if ( ioctl ( this -> dspDev, SNDCTL_DSP_SETFMT, &FORMAT ) == -1 ) {
      perror ( "SNDCTL_DSP_SETFMT" );
      return false;
//       exit ( -1 );
   }
// 
   if ( ioctl ( this -> dspDev, SNDCTL_DSP_CHANNELS, &NCHANS ) == -1 ) {
      perror ( "SNDCTL_DSP_CHANNELS" );
      return false;
//       exit ( -1 );
   }

   if ( ioctl ( this -> dspDev, SNDCTL_DSP_SPEED, &RATE ) == -1 ) {
      perror ( "SNDCTL_DSP_SPEED" );
      return false;
//       exit ( -1 );
   }
   
//    if ( ioctl(this -> dspDev, SNDCTL_DSP_SETFRAGMENT, &frag) < 0 ) {
//       perror("SNDCTL_DSP_SETFRAGMENT");
//    }
   return true;
}
