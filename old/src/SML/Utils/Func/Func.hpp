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

#ifndef __SS_SILENTMEDIA_FUNC_HPP_
#define __SS_SILENTMEDIA_FUNC_HPP_

#include <SML/slib.hpp>

namespace Utils {
   namespace Func {
      std::string toUpper ( std::string strToConvert );
      std::string toLower ( std::string strToConvert );

      std::string getFileNamefromFullPath ( const std::string& str );
      std::string getFolderNamefromFullPath ( const std::string& str );

      long int getFileSize ( std::string inputFile );
   }
}

#endif
