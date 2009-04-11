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

#ifndef __SS_SILENTMEDIA_PAI_HPP_
#define __SS_SILENTMEDIA_PAI_HPP_

#include <lua.hpp>
#include <SML/sml.hpp>

namespace SilentMedia {
   class Action {
      public:
         Action ( void );
         ~Action ( void );
         
         static Action * Instance ( void );
         void loadMap ( std::string map );
         lua_State * getMethod ( std::string object, std::string method );
         void call ( std::string object, std::string method );
         void play ( void );
         long int timer ( int val );
         void addNew ( std::string );
         void addFile ( std::string );
      private:
         static Action * _action;
         Audio * audio;
         DecodedData * ddata;
         lua_State * L;
         
         std::string map;
         std::string mapStringParam [ 7 ];
         int mapIntParam [ 7 ];
         std::map < std::string, std::string > idToName;
         std::map < std::string, std::string > _idToName;
         std::map < std::string, double > timeMark;
         std::map < std::string, int > countMap;
         std::map < double, std::string > timeMap;
         std::multimap < double, std::string > timeMapRelative;
   };
}

#endif
