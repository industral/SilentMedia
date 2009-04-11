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

#ifndef __SS_SILENTMEDIA_WAVPACK_HPP_
#define __SS_SILENTMEDIA_WAVPACK_HPP_

#include <SML/sml.hpp>
#include <SML/Audio/Codec/AbstractCodec.hpp>

#include <wavpack/wavpack.h>

namespace SilentMedia {
   class DecodedData;
   namespace Codec {
      class WavPack : virtual public AbstractCodec {
      public:
         WavPack ( void );
         virtual ~WavPack ( void );

         // virtual
         virtual bool init ( std::string inputfile );
         virtual void play ( void );
         virtual unsigned long int getCurrSeekPos ( void ) const;
         virtual void setSeekPos ( double seekPos );
         virtual void flush ( void );
         virtual void finish ( void );
      private:
         inline void releaseBuff ( void );

         DecodedData * ddata;
         int dspDev;
         
         WavpackContext * wpc; // Для инициализации
         WavpackContext * pwpc; // Для проигрования
         std::string fileName;

         const unsigned int bufSize;

         // стражи
         bool seekCheck;
         bool playCheck;

         int fileSize;
         int totalTime;

         int32_t * buffer;
         int16_t * outputBuf;
   };
}
}

#endif
