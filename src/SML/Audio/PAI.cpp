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

#include <SML/Audio/Audio.hpp>
#include <SML/Audio/Codec/DecodedData.hpp>
#include "PAI.hpp"

namespace SilentMedia {
   Action * Action ::_action = NULL;
   
   Action::Action ( void ) : audio ( Audio::Instance() ), ddata ( DecodedData::Instance() ) { }
   Action::~Action ( void ) {
      lua_close ( this -> L );
   }
   
   Action * Action::Instance ( void ) {
      if ( _action == NULL ) {
         _action = new Action();
      }
      return ( _action );
   }
   
   void Action::loadMap ( std::string map ) {
      this -> map = map;
      this -> L = lua_open();
      luaL_openlibs  ( this -> L );
      if ( luaL_loadfile ( this -> L, map.c_str() ) || lua_pcall ( this -> L, 0, 0, 0 ) ) {
         lua_error ( this -> L );
      }
   }
   
   lua_State * Action::getMethod ( std::string object, std::string method ) {
      // object - выбираем объект
      lua_getfield ( this -> L, LUA_GLOBALSINDEX, object.c_str() );
      if ( !lua_istable ( this -> L, -1 ) ) {
         luaL_error ( this -> L, ( object + " is not a valid object" ).c_str() );
      }
      
      // object[method] - выбираем метод
      lua_getfield ( this -> L, -1, method.c_str() );
      if ( !lua_istable ( this -> L, -1 ) ) {
         luaL_error ( this -> L, ( method + " is not a valid method" ).c_str() );
      }
      
      return ( this -> L );
   }
   
   void Action::call ( std::string object, std::string method ) {
      this -> loadMap ( this -> map );
      lua_State * l = getMethod ( object, method );

      std::map < int, std::string > l_name;
      std::map < int, std::string > l_id;
      std::map < int, std::string > _l_id;
      std::map < int, std::string > l_next;
      std::map < int, int > l_count;
      std::map < int, int > l_offset;

      // Получить количество таблиц в методе
      int count = lua_objlen ( l, -1 );
      std::cout << count << std::endl;

      // выборка значений с таблиц
      for ( int i = 1; i <= count; ++i ) {
         for ( int k = 1; k <= 5; ++k ) {
            l = getMethod ( object, method );
            // object[method][i] - номер таблицы
            lua_rawgeti ( l, -1, i );
            // object[method][i][i] - номер значения в таблицы
            lua_rawgeti ( l, -1, k );
         
            // Перебираем все 5 пераметров в таблице
            if ( k == 1 || k == 2 || k == 3 ) { // 1 -3 это string
               this -> mapStringParam [ k ] = (std::string) lua_tostring ( this -> L, -1 );
//                std::string result = mapStringParam [ k ];
//                std::cout << "result: " << result << std::endl;
            } else if ( k == 4 || k == 5 ) { // 4 - 5 - это int
               this -> mapIntParam [ k ] = ( int ) lua_tonumber ( this -> L, -1 );
//                int result = mapIntParam [ k ];
//                std::cout << "result: " << result << std::endl;
            }
         }
         
         /*
         mapStringParam [ 1 ] - название файла
         mapStringParam [ 2 ] - идентификатор файла
         mapStringParam [ 3 ] - файл смещения ( файл, относительно которого делается смещение )
         mapIntParam [ 4 ] - количество циклов воспроизведения
         mapIntParam [ 5 ] - смещение по времени
         */

         l_name [ i ] = mapStringParam [ 1 ];
         l_id [ i ] = mapStringParam [ 2 ];
         l_next [ i ] = mapStringParam [ 3 ];
         l_count [ i ] = mapIntParam [ 4 ];
         l_offset [ i ] = mapIntParam [ 5 ];
         
         std::cout << "offset: " << l_offset [ i ] << std::endl;
         std::cout << "l_next: " << l_next [ i ] << std::endl;
         std::cout << "l_id: " << l_id [ i ] << std::endl;
         
         this -> idToName [ l_id [ i ] ] = l_name [ i ];
         this -> countMap [ l_id [ i  ] ] = l_count [ i ];
//          _l_id [ i ] = l_id [ i ];
//                this -> audio -> openFile ( l_name [ i ], _l_id [ i ] );
//             this -> _idToName [ l_id [ i ] ] = l_name [ i ];
//          this -> timeMark [ l_id [ i ] ] =  ( this -> ddata -> getTotalTime() + l_offset [ i ] );
               // Если смещение отрицательное - то смещение относительно начала воспроизведения файла
               // иначе относительно конца воспроизведения файла
         if ( l_offset [ i ] == 0 ) {
//             this -> timeMapRelative [ 0 ] = l_id [ i ];
            this -> timeMapRelative . insert ( std::pair < double, std::string > ( 0, l_id [ i ] ) );
            this -> timeMap [ 0 ] = l_id [ i ];
            this -> timeMark [ l_id [ i ] ] = 0;
            std::cout  << "0: " << l_id [ i ] << std::endl;
         } else if ( l_offset [ i ] < 0 ) { // если отрицательное
            /*
               то смещение - относительно начала первой метки файла смещения.
               файл смещения не может быть пустым. Если он пуст, то смещение не может быть отрицательное.
            */
            if ( !l_next [ i ] . empty() ) {
//                std::cout << "CORE1: " << l_id [ i ] << std::endl;
               this -> timeMark [ l_id [ i ] ] = ( this -> timeMark [ l_next [ i ] ] + -l_offset [ i ] );
               this -> timeMap [ this -> timeMark [ l_id [ i ] ] ] = l_id [ i ];
//                this -> timeMapRelative [ -l_offset [ i ] ] = l_id [ i ];
               this -> timeMapRelative . insert ( std::pair < double, std::string > ( -l_offset [ i ], l_id [ i ] ) );
            } else {
               std::cout << "Parameter of offset file is empty. Can't use negative offset!" << std::endl;
            }
         } else { // если смещение положительное
               // тогда добавляем смещение к общей длительности файла смещения
            double offset = -1;
//             std::cout << "CORE2: " << l_id [ i ] << std::endl;
            if ( !l_next [ i ] . empty() ) {
               _l_id [ i ] = l_next [ i ];
               this -> audio -> openFile ( this -> idToName [ l_next [ i ] ], _l_id [ i ] );
               offset = ( this -> ddata -> getTotalTime() * 1e6 + l_offset [ i ] );
               std::cout << "TOTAL TIME: " << this -> idToName [ l_next [ i ] ] << " : " << this -> ddata -> getTotalTime() << std::endl;
               this -> audio -> closeF ( _l_id [ i ] );
               this -> audio -> destroyObj ( _l_id [ i ] );
            } else {
               offset = l_offset [ i ];
            }
            this -> timeMark [ l_id [ i ] ] =  ( offset );
            this -> timeMap [ offset ] = l_id [ i ];
//             this -> timeMapRelative [ offset ] = l_id [ i ];
            this -> timeMapRelative . insert ( std::pair < double, std::string > ( offset, l_id [ i ] ) );
         }
//          this -> timeMark [ l_id [ i ] ] =  ( this -> ddata -> getTotalTime() + l_offset [ i ] );
//          this -> audio -> closeF ( _l_id [ i ] );
//          this -> audio -> destroyObj ( _l_id [ i ] );
         
         // передаем полученые параметры
//          audio -> play ( mapStringParam [ 1 ], mapStringParam [ 2 ], mapStringParam [ 3 ],
//                          mapIntParam [ 4 ], mapIntParam [ 5 ] );
         lua_pop ( l, -1 ); // очищаем стек
      }
      this -> play();
      this -> idToName . clear();
      this -> timeMark . clear();
      this -> timeMapRelative . clear();
   }
   
   void Action::play ( void ) {
      /*static */std::map < double, std::string >::iterator it;
//                  std::map < double, std::string >::iterator it2;
//                  std::pair < std::multimap < double, std::string >::iterator, 
//                  std::multimap < double, std::string >::iterator > ret;

//       for (;;) {
//       for ( int i = 0; i <= this -> timeMark . begin() -> second; usleep
//       long double time = 0;
//       while ( time < this -> timeMark . begin() -> second ) {
//          time += 1000000;
//          usleep ( 1000000 );
//          std::cout << time << std::endl;
//       }
                 for ( it = this -> timeMapRelative . begin(); it != this -> timeMapRelative . end(); ++it ) {
         std::cout << "PLAY: " << this -> idToName [ it -> second ] << std::endl;
         std::cout << "PLAY: " << it -> first << std::endl;
         std::cout << "PLAY: " << it -> second << std::endl;
//          
//          for ( it2 = ret.first; it2!=ret.second; ++it2) {
            if ( it -> first > 0 ) {
               usleep ( it -> first );
            }
            if ( l_count
            audio -> play ( this -> idToName [ it -> second ], it -> second, "stuff",
                            -1, 0 );
//             cout << " " << (*it).second;
//          cout << endl;
//                  }


         
// //          this -> timer ( 10 );
//       std::cout << "it -> second: " << it -> second << std::endl;
//       ++it;
//          audio -> play ( this -> idToName [ it -> first ], it -> first, "stuff",
//                          -1, 0 );
      }
//       if ( this -> timeMark . find ( time );
   }
   
   long int Action::timer ( int val ) {
      static long double time = 0;
      time += 100000;
      usleep ( 100000 );
      std::cout << "TIME: " << time << std::endl;
   }
   
   void Action::addNew ( std::string actionName ) {
   }
   
   void Action::addFile ( std::string fileName ) {
   }
   
}
