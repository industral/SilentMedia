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

#ifndef __SS_SILENTMEDIA_OSS_DSP_HPP_
#define __SS_SILENTMEDIA_OSS_DSP_HPP_

#include <SML/config.hpp>
#include <SML/Audio/SoundSystem/AbstractSoundSystem.hpp>

namespace SilentMedia {
   namespace SoundSystem {
      namespace OSS {
//       enum Status { Stop, Play, Pause, Resume, Active };
         class DSP : virtual public AbstractSoundSystem {
            public:
               DSP ( void );
               virtual ~DSP ( void );

               static SilentMedia::AbstractSoundSystem * Instance ( void );

               virtual bool init ( std::string dspDev );
               virtual bool setParams ( int channels, int sampleRate );
               virtual bool play ( const void * buf );
//                virtual int getDSPDev ( void ) const;
//                virtual void suspend ( void );
//                bool play ( void );
            private:
               static DSP * _dsp;

               int dspDev;  // файловый дескриптор /dev/dsp
               int input_fd; // файловый дескриптор воспроизводящегося файла
         };
      }
   }
}

#endif
