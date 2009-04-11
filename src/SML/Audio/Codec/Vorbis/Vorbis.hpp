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

#ifndef __SS_SILENTMEDIA_VORBIS_HPP_
#define __SS_SILENTMEDIA_VORBIS_HPP_

#include <SML/sml.hpp>
#include <SML/Audio/Codec/AbstractCodec.hpp>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace SilentMedia {
   class DecodedData;
   namespace Codec {
      class Vorbis : virtual public AbstractCodec {
         public:
            Vorbis ( void );
            virtual ~Vorbis ( void );

         // virtual
            virtual bool init ( std::string inputfile );
            virtual void play ( void );
            virtual unsigned long int getCurrSeekPos ( void ) const;
            virtual void setSeekPos ( double val );
            virtual void flush ( void );
            virtual void finish ( void ) { }; // flush
         private:
            void readVorbisComment ( void );

            OggVorbis_File vf;
            OggVorbis_File pvf;
         
            DecodedData * ddata;
            int dspDev;
         
            vorbis_info *vi;

            double length;
            double seekPos;
            bool seek;
            std::string fileName;
      };
   }
}

#endif
