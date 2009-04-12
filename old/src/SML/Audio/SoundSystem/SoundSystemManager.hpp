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

#ifndef __SS_SILENTMEDIA_SOUNDSYSTEMMANAGER_HPP_
#define __SS_SILENTMEDIA_SOUNDSYSTEMMANAGER_HPP_

#include <SML/sml.hpp>

namespace SilentMedia {
   namespace SoundSystem {
      namespace OSS {
         class DSP;
      }
   }
   class AbstractSoundSystem;
   class SoundSystemManager {
      public:
         ~SoundSystemManager ( void );
         static SoundSystemManager * Instance ( void );
         bool init ( AudioComponents component, SoundSystemType soundSystem, std::string dspDev,
                     std::string mixerDev );
         
         AbstractSoundSystem * getNeedObj ( void );
         void allocateQuery ( std::string id );
         void deAllocateQuery ( std::string id );
         bool play ( const void * buf, std::string id );
         bool setParams ( unsigned short int channels, unsigned int sampleRate );
//          bool initMultiPlay ( void );
//          void setSoundSystem ( SoundSystemType soundSystem ) { this -> soundSystem = soundSystem; }
//          void setDSPDev ( std::string dspDev ) { this -> dspDev = dspDev; }
      private:
         SoundSystemManager ( void );
         
         static SoundSystemManager * _ssystem;
         SoundSystemManager * ssystem;
         AbstractSoundSystem * dsp;
         
//          EngineType engineType;
//          AudioComponents component;
//          SoundSystemType soundSystem;
         std::string id;
         std::string dspDev;
         std::vector < int > dspList;
         std::map < std::string, int > dspMap;
//          std::string mixerDev;

         std::map < std::string, AbstractSoundSystem * > objs;
         std::map < int, AbstractSoundSystem * > dspObjs;
         SoundSystemType selectedSoundSystem;
//          SoundSystemManager * dspObj;
//          soundSystemList ssystem;
   };
}



#endif
