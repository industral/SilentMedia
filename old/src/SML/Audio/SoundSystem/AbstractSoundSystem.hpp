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

#ifndef __SS_SILENTMEDIA_ABSTRACTSOUNDSYSTEM_HPP_
#define __SS_SILENTMEDIA_ABSTRACTSOUNDSYSTEM_HPP_

#include <SML/slib.hpp>

namespace SilentMedia {
   class AbstractSoundSystem {
      public:
//          AbstractSoundSystem ( void );
         virtual ~AbstractSoundSystem ( void ) { };

//          virtual AbstractSoundSystem * Instance ( void ) = 0;
         virtual bool init ( std::string /*_Dev*/ ) = 0;
         virtual int getDSPDev ( void ) const { return -1; };
         virtual bool play ( const void * /*buf*/ ) { return false; };
         virtual bool setParams ( unsigned short int /*channels*/, unsigned int /*sampleRate*/ ) { return false; };

         virtual bool start ( void ) { return false; }
         virtual bool stop ( void ) { return false; }
      private:
   };
}

#endif
