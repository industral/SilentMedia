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

#include "ALSA_DSP.hpp"

SilentMedia::SoundSystem::ALSA::DSP::DSP ( void ) : thread1 ( -1 )
{ }

SilentMedia::SoundSystem::ALSA::DSP::~DSP ( void ) {
   snd_pcm_close ( this -> handle );
}

bool SilentMedia::SoundSystem::ALSA::DSP::init ( std::string dev ) {
   std::cout << "In ALSA::DSP::init: " << this << std::endl;
   this -> pcmDev = dev;
   int err = -1;
   if ( ( err = snd_pcm_open ( &this -> handle, this -> pcmDev.c_str(), SND_PCM_STREAM_PLAYBACK,
          /*SND_PCM_NONBLOCK |*/ SND_PCM_ASYNC ) ) < 0 ) {
             std::cout << "Playback open error: " << snd_strerror ( err ) << std::endl;
             exit ( EXIT_FAILURE );
          }
//           snd_pcm_nonblock(handle, 1);
          return true;
}

bool SilentMedia::SoundSystem::ALSA::DSP::setParams ( unsigned short int channels,
      unsigned int sampleRate ) {
      int err = -1;
      if ( ( err = snd_pcm_set_params ( this -> handle, SND_PCM_FORMAT_S16, SND_PCM_ACCESS_RW_INTERLEAVED,
             channels, sampleRate, 1, /*1000000*/ 500000 ) ) < 0 ) {
                std::cout << "Playback open error: " << snd_strerror ( err ) << std::endl;
                exit ( EXIT_FAILURE );
             }
   return true;
}

bool SilentMedia::SoundSystem::ALSA::DSP::start ( void ) {
   return !( snd_pcm_prepare ( this -> handle ) );
}

bool SilentMedia::SoundSystem::ALSA::DSP::stop ( void ) {
   return true;
}

/*
   -EAGAIN: -11
   -EPIPE: -32
   -ESTRPIPE: -86
*/

bool SilentMedia::SoundSystem::ALSA::DSP::play ( const void * buf ) {
   snd_pcm_sframes_t frames = 0;
   frames = snd_pcm_writei ( this -> handle, buf, 4096 );
//    std::cout << "frames: " << frames << std::endl;
   return true;
}

