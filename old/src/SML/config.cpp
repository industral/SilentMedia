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

#include "config.hpp"

namespace SilentMedia {
   bool _debug_ ( std::string name, const char *f, int l ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         std::cout << std::endl << "[libsml v." << SILENTMEDIA_VERSION << "]: " << "in " << "`" << f 
               << "`" << " line: " << l << std::endl;
//       std::cout << "DEBUG INFO: " << "dsp_fd = " << dsp_fd << std::endl;
         perror ( name . c_str() );
      }
      if ( SILENTMEDIA_STRICT == true ) {
//          exit ( 1 );
      }
      return false;
   }
}
