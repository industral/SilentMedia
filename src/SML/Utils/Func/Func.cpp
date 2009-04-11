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

#include "Func.hpp"

// конвертирует символы в строке в верхний регистр
std::string Utils::Func::toUpper ( std::string strToConvert ) {
   for ( unsigned int i = 0; i != strToConvert . length(); ++i ) {
      strToConvert [ i ] = toupper ( strToConvert [ i ] );
   }
   return strToConvert;
}

// конвертирует символы в строке в нижний регистр
std::string Utils::Func::toLower ( std::string strToConvert ) {
   for ( unsigned int i = 0; i != strToConvert . length(); ++i ) {
      strToConvert [ i ] = tolower ( strToConvert [ i ] );
   }
   return strToConvert;
}

/*
   http://www.cplusplus.com/reference/string/string/find_last_of.html
*/

// возвращает имя файла из полного пути
std::string Utils::Func::getFileNamefromFullPath ( const std::string& str ) {
   size_t found;
   found = str . find_last_of ( "/" );
   return ( str . substr ( found + 1 ) );
}

// возвращает путь вплоть до имя папки ( без имени файла )
std::string Utils::Func::getFolderNamefromFullPath ( const std::string& str ) {
   size_t found;
   found = str . find_last_of ( "/" );
   return ( str . substr ( 0, found ) );
}

// возвращает размер файла
long int Utils::Func::getFileSize ( std::string inputFile ) {
   long int begin = -1;
   long int end = -1;
   std::ifstream myfile ( inputFile.c_str() );
   begin = myfile.tellg();
   myfile.seekg ( 0, std::ios::end );
   end = myfile.tellg();
   myfile.close();

   return ( end - begin );
}
