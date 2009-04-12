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

#ifndef __SS_SILENTMEDIA_DECODEDDATA_HPP_
#define __SS_SILENTMEDIA_DECODEDDATA_HPP_

#include <SML/sml.hpp>

namespace SilentMedia {
   class SoundSystemManager;
   class Audio;
   class DecodedData {
      public:
         ~DecodedData ( void );
         
         static DecodedData * Instance ( void );
         
         void begin ( std::string id );
         void end ( std::string id );
         
         void setMainParam ( std::string fileName
               , long int fileSize = -1
               , double totalTime = -1
               , int channels = -1
               , int sampleRate = -1
               , double bitRate = -1
               , int bitsPerSample = -1
                           );

         void clean ( void );
//          bool setParams ( void );

         std::map < std::string, std::string > getVorbisComment ( void ) {
            return ( this -> vorbisComm );
         }

         std::multimap < ::FLAC__StreamMetadata_Picture_Type, std::string > getPicture ( void ) {
            return ( this -> picData );
         }

         void setPicture ( std::multimap < ::FLAC__StreamMetadata_Picture_Type, std::string > picData ) {
            this -> picData = picData;
         }

         void setVorbisComment ( std::map < std::string, std::string > vorbisComm ) {
            this -> vorbisComm = vorbisComm;
         }

         // get func
         unsigned long int getFileSize ( void ) { return ( this -> fileSize ); }
         double getTotalTime ( void ) { return ( this -> totalTime ); }
         unsigned int getChannels ( void ) { return ( this -> channels ); }
         unsigned int getSampleRate ( void ) { return ( this -> sampleRate ); }
         double getBitRate ( void ) { return ( this -> bitRate ); }
         short int getBitsPerSample ( void ) { return ( this -> bitsPerSample ); }

         bool play ( const void * buf, std::string id );
//          int getDSPDev ( void ) const;
      private:
         DecodedData ( void );

         static DecodedData * _ddata;
         SoundSystemManager * ssystem;
         Audio * audio;

         bool playCheck;

         std::map < std::string, std::map < std::string, void * > > archive;
         std::string fileName;
         long int fileSize;
         double totalTime;
         int channels;
         int sampleRate;
         double bitRate;
         int bitsPerSample;

         std::multimap < ::FLAC__StreamMetadata_Picture_Type, std::string > picData;
         std::map < std::string, std::string > vorbisComm; // Vorbis Comment

         std::map < std::string, std::string > vorbisTags;
         std::map < std::string, std::string > coverData;

   };
}

#endif
