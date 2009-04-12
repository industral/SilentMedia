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

#ifndef __SS_SILENTMEDIA_ALSA_DSP_HPP_
#define __SS_SILENTMEDIA_ALSA_DSP_HPP_

#include "alsa/asoundlib.h"
#include <SML/config.hpp>
#include <SML/Audio/SoundSystem/AbstractSoundSystem.hpp>

namespace SilentMedia {
   namespace SoundSystem {
      namespace ALSA {
//       enum Status { Stop, Play, Pause, Resume, Active };
         class DSP : virtual public AbstractSoundSystem {
            public:
               DSP ( void );
               virtual ~DSP ( void );

               static SilentMedia::AbstractSoundSystem * Instance ( void );

               virtual bool init ( std::string dev );
               virtual bool setParams ( unsigned short int channels, unsigned int sampleRate );
               virtual bool play ( const void * buf );
//                virtual bool suspend ( void );
               
               virtual bool start ( void );
               virtual bool stop ( void );
            private:
//                static DSP * _dsp;

               snd_pcm_t * handle;
               std::string pcmDev;
               pthread_t thread1;
         };
      }
   }
}

#endif
