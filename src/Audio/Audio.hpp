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

#ifndef __SS_SILENTMEDIA_AUDIO_HPP_
#define __SS_SILENTMEDIA_AUDIO_HPP_

// #include <SML/sml.hpp>
// #include "Codec/AbstractCodec.hpp"
// #include <SML/Audio/SoundSystem/OSS/DSP/dsp.hpp>
// #include <SML/Audio/Codec/AbstractCodec.hpp>

// #include "SoundSystem/OSS/OSS.hpp"

#include <SML/sml.hpp>
#include <SML/slib.hpp>


// #include <SML/Audio/Codec/DecodedData.hpp>

namespace SilentMedia {
   enum Status { Stop, Play, Pause, Resume, Active };
//    class DSP;
   class SoundSystemManager;
   class DecodedData;
   class AbstractCodec;
   class Audio {
      public:
         ~Audio ( void );

         static Audio * Instance ( void );
         bool init ( EngineType engineType, AudioComponents component, SoundSystemType soundSystem, std::string dspDev = "/dev/dsp",
                     std::string mixerDev = "/dev/mixer");
//          bool initMultiPlay ( void );

         bool play ( std::string inputFile, std::string id, std::string next = NULL,
                     int cycleCount = -1, unsigned int pauseDelay = 0 );
         bool destroyObj ( std::string id );
            
         SilentMedia::AbstractCodec * getNeedObj ( void );
         bool openFile ( std::string inputFile, std::string& id);
         void closeF ( std::string id );
         void closeF ( void );
         void flush ( void );
         void finish ( void );
         
         bool changeStatus ( Status myStatus );

         void play_ ( std::string id );

         void setSeek ( int val );
         double getSeek ( void );
      private:
         Audio ( void );

         static Audio * _audio;
         Audio * audio;
         SoundSystemManager * ssystem;
         DecodedData * ddata;
         EngineType engineType;

         std::map < std::string, SilentMedia::AbstractCodec * > objs;
         std::map < std::string, SilentMedia::AbstractCodec * > idObjs;
         std::map < std::string, pthread_t > threadMap;
         std::set < std::string > idList;

//          std::string id;
         std::string ext;
         std::string playExt;
         std::string inputFile;
//          std::string objId;

         Status songStatus;

         pthread_t thread1;
   };
}

#endif
