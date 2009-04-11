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

#ifndef __SS_SILENTMEDIA_ABSTRACTCODEC_HPP_
#define __SS_SILENTMEDIA_ABSTRACTCODEC_HPP_

// #include "../Audio.hpp"
// #include <SML/sml.hpp>

#include <SML/slib.hpp>

// используем макрос а не ф-цию, так как если ф-ция const то ф-ция не подойдет
#define PRINT(funcName) ( std::cout << "WARNING: " << (funcName) << " stuff function! Please redefine it." << std::endl )

namespace SilentMedia {
//          class DSP;
   /// Абстрактный класс для последуещего наследования классами FlacDecode, WavDecode, VorbisDecode, WavpackDecode
   class AbstractCodec {
      public:
         AbstractCodec ( void ) /*: dsp ( new DSP )*/ { };
         virtual ~AbstractCodec ( void ) { };
         // Делаем одну ф-цию чистой виртуальной чтобы сделать класс абстрактным. Тем более что это ф-ция уж точно должна быть переопределена :)
         /// Инициализация файла.
         /** Главная ф-ция для инициализация файла: его открытия, получения различного рода информации ( комментарии ( если они поддерживаются ),
          *  технической информации ( размер файла, битрейт, частота дискретизации и т.д. ) ).
          *  Если информации получена, и файл больше не нуждается в использовании ( напр. планируется инициализировать другой файл в целях получить
          *  информацию с него, то обязательно закрываем файловый дискриптор, и освобождаем ресурсы ф-цией closeF() */
         /// @param std::string inputfile - имя файла для инициализации
         /// @return true - успешна инициализация, false - ошибка инициализации
         virtual bool init ( std::string /*inputfile*/, std::string /*idObj*/ ) { return true; }
         /// Начать воспроизведение файла
         virtual bool play ( std::string /*id*/ ) { /*PRINT ( "play()" );*/ return false ; }
         // возвращаем 0 не а true, ибо это должно быть значение, тем более что это unsigned
         /// Получить текущею позицию воспроизводящегося файла
         /// @return Текущея позиция в процентах от продолжительности всего файла
         virtual unsigned long int getSeek ( void ) const { /*PRINT ( "getCurrSeekPos()" );*/ return 0; }
         /// Установить текущею позицию
         virtual void setSeek ( double /*val*/ ) { /*PRINT ( "setSeekPos()" );*/ }
//         /// Закрыть файловый дискриптор
         virtual void closeF ( void ) { /*PRINT ( "close()" );*/ }
         /// Освободить системные ресурсы
         virtual void flush ( void ) { /*PRINT ( "flush()" );*/ }
         /// Завершить работу
         virtual void finish ( void ) { /*PRINT ( "finish()" );*/ }
//          int * getDSP ( void ) { return ( this -> dsp -> retDSP() ); }
//          void setV ( int val ) { this -> checkVal = val; }
//          int getV ( void ) { return this -> checkVal; }
      protected:
//          DSP * dsp;
//          int checkVal;
   };
}

#endif
