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

// API http://manuals.opensound.com/developer/MIXER.html

#include "OSS_Mixer.hpp"

SilentMedia::SoundSystem::OSS::Mixer * SilentMedia::SoundSystem::OSS::Mixer ::_mixer = NULL;

SilentMedia::SoundSystem::OSS::Mixer * SilentMedia::SoundSystem::OSS::Mixer::Instance ( void ) {
   if ( _mixer == NULL ) {
      _mixer = new SilentMedia::SoundSystem::OSS::Mixer();
   }
   return ( _mixer );
}

SilentMedia::SoundSystem::OSS::Mixer::Mixer ( void  ) : default_mixer_dev ( -1 ), mixer_fd ( -1 ), nrext ( -1 )
{ }

/// Перегруженый конструктор
/// @param dev - имя микшерного устройства ( напр. "/dev/mixer" )
SilentMedia::SoundSystem::OSS::Mixer::Mixer ( std::string dev ) {
   this -> init ( dev );
}

SilentMedia::SoundSystem::OSS::Mixer::~Mixer ( void ) { }

/// Инициализация микшера
/**
   @param mixer название микшерного устройства ( "/dev/mixer" )
   @return 1 - успех, 0 - неудача

   \code
   Mixer * ossmix = new Mixer;
   ossmix -> init( "/dev/mixer" );
   \endcode
*/
bool SilentMedia::SoundSystem::OSS::Mixer::init ( std::string dev ) {
   if ( ( mixer_fd = open ( dev . c_str(), O_RDWR ) ) == -1 ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         return ( _debug ( dev ) );
      }
   }

   this -> findDefaultMixerDev();
   this -> _OSS_NREXT();
//    this -> MI();
//    this -> _OSS_SNDCTL_SYSINFO();
//    this -> _OSS_AUDIOINFO();
   this -> getControlList();

   return true;
}

int SilentMedia::SoundSystem::OSS::Mixer::getUpdateCounter ( void ) {
   this -> MI();
   return mi . modify_counter;
}

/**
   Создает список доступных устройств ( к примеру, для справки: get, set ) \n
   @return Список доступных контролеров в системе, разделенных комой. \n
   К прим. \n
   \code
   std::string comaSeparatedListOfControls;
   ossmix -> comaSeparatedListOfControls ( comaSeparatedListOfControls );
   \endcode
   даст нам: \n
   \code
   aux1, cd, center, igain, line, mic, mono, pcm, phone, rear, speaker, video
   \endcode
*/
bool SilentMedia::SoundSystem::OSS::Mixer::comaSeparatedListOfControls ( std::string &controlList, char param ) {
// tmpFullAvaibleMixDev.clear();
   std::map < int, std::string > container;
   if ( param == 'W' ) {
      container = listOfWriteableCtrlDev;
   } else if ( param == 'R' ) {
      container = listOfReadableCtrlDev;
   }

   std::string list;
   unsigned int i = 0;
   for ( std::map < int, std::string >::iterator it = container . begin(); it != container . end(); ++it ) {
      ++i;
      list = it -> second;
      if ( i == container . size() ) {
         controlList += list;
         return true;
      } else {
         controlList += list + ", ";
      }
   }
   return false;
}

void SilentMedia::SoundSystem::OSS::Mixer::getParentLabelByNum ( int parentNum, std::string &parentLabel, unsigned int &parentCount ) {
   parentLabel = fullCtrlList . find ( parentNum ) -> second;
   parentCount =  fullCtrlList . size();
}

void SilentMedia::SoundSystem::OSS::Mixer::getParentNumByCtrlNum ( int ctrlNum, unsigned int &parentNum, unsigned int &prevParentNum, unsigned int &nextParentNum ) {
   parentNum = parentList . find ( ctrlNum ) -> second;
   prevParentNum = -- ( parentList . find ( ctrlNum ) ) -> second;
   nextParentNum = ++ ( parentList . find ( ctrlNum ) ) -> second;
// std::cout << "LSSOSS: " << ctrlNum << " : " << parentNum << " : " << prevParentNum << " : " << nextParentNum << std::endl;
}

// bool SilentMedia::SoundSystem::OSS::Mixer::getFullControlList() {
// // tmpFullAvaibleMixDev.clear();
//    for ( int i = 1; i < nrext; i++ ) {
//       unsigned short int ctrlNum = i;
//       EXT ( ctrlNum );
//
//       if ( ext.type != MIXT_MARKER ) {
// //       std::cout << ext.extname << " : " << ext.parent << " : " << ext.ctrl << std::endl;
//          fullCtrlList [ i ] = ext.parent;
//       }
//    }
//    return 1;
// }

/// Получить список всех доступных контролеров
/**
   Метод для получения доступа к std::map < int, std::string > со списком всех доступных контролеров \n
   Принимает и возвращает std::map в качестве параметра \n
   \code
   std::map < int, std::string > controlList;
   ossmix -> returnListOfAvaibleControlDev ( controlList );
   \endcode
*/
void SilentMedia::SoundSystem::OSS::Mixer::returnListOfAvaibleControlDev ( std::map < int, std::string > &listOfAvaibleCtrlDev ) {
   listOfAvaibleCtrlDev = this -> listOfAvaibleCtrlDev;
}

/// Получить номер контролера по его имени
/**
   @return Номер контролера по его имени

   \code
   // Узнаем номер контролера с именем "vol"
   int ctrlNum; // Обявление переменной, сюда будет возвращен номер найденого контролера
   std::string controlName = "vol"; // Указываем искомое имя
   ossmix -> getNumByCtrlName ( controlName, ctrlNum );
   std::cout << ctrlNum << std::endl;
   \endcode
*/
bool SilentMedia::SoundSystem::OSS::Mixer::getNumByCtrlName ( std::string controlName, int &ctrlNum ) {
   for ( std::map < int, std::string >::iterator it = listOfAvaibleCtrlDev . begin(); it != listOfAvaibleCtrlDev . end(); ++it ) {
      if ( it -> second == controlName ) {
         ctrlNum = it -> first;
         return true;
      }
   }
   return false;
}

/// Получить имя контролера по его номеру
/**
   @return Имя контролера по его номеру: \n

   \code
   // Узнаем имя контролера с номером "2"
   int ctrlNum = 2; // Инициализация искомой переменной
   std::string controlName;  // Обявление переменной, сюда будет возвращено имя найденого контролера
   ossmix -> getNameByCtrlNum ( ctrlNum, controlName );
   std::cout << controlName << std::endl;
   \endcode

*/
bool SilentMedia::SoundSystem::OSS::Mixer::getNameByCtrlNum ( int ctrlNum, std::string &controlName ) {
   for ( std::map < int, std::string >::iterator it = listOfAvaibleCtrlDev . begin(); it != listOfAvaibleCtrlDev . end(); ++it ) {
      if  ( it->first == ctrlNum ) {
         controlName = it -> second;
         return true;
      }
   }
   return false;
}

/// Проверка контролера на возможность записи
/**
   Проверяет способность устройства на запись \n
   Принимает: имя устройства \n
   @return: Номер устройства, 1 - запись доступна, 0 - запись недоступна \n

   \code
   // Проверяем способность контролера "mic" на запись
   std::string controlName = "mic";
   int ctrlNum; // в ctrlNum будет возвращен номер контролера
   ossmix -> checkRecAvail ( controlName, ctrlNum );
   std::cout << ctrlNum << std::endl;
   \endcode

*/
bool SilentMedia::SoundSystem::OSS::Mixer::checkRecAvail ( std::string controlName, int &ctrlNum ) {
   std::string str = controlName + ".rec";

   for ( std::map < int, std::string >::iterator it = listOfAvaibleCtrlDev . begin(); it != listOfAvaibleCtrlDev . end(); ++it ) {
      if ( it->second == str ) {
         ctrlNum = it -> first;
         return true;
      }
   }
   return false;
}

/// Перегруженая ф-ция. @see  bool SilentMedia::SoundSystem::OSS::Mixer::checkRecAvail ( std::string controlName, int &ctrlNum ) \n
/**
   Принимает: имя контролера \n
   @return: 1 - запись доступна, 0 - запись недоступна \n
*/
bool SilentMedia::SoundSystem::OSS::Mixer::checkRecAvail ( std::string param ) {
   int ctrlNum = 0;
   std::string controlName;
   return checkRecAvail ( controlName, ctrlNum );
}

/// Перегруженая ф-ция. @see  bool SilentMedia::SoundSystem::OSS::Mixer::checkRecAvail ( std::string controlName, int &ctrlNum ) \n
/**
   Принимает: номер контролера \n
   @return: 1 - запись доступна, 0 - запись недоступна \n
*/
bool SilentMedia::SoundSystem::OSS::Mixer::checkRecAvail ( int ctrlNum ) {
   std::string controlName;
   getNameByCtrlNum ( ctrlNum, controlName );
   return checkRecAvail ( controlName );
}

/// Получить различную информацио о контролере
/**
   Возвращает различную информацию об контролере: \n
   @param ctrlNum - номер контролера в системе \n
   @param ctrlLabel - имя контрлера \n
   @param numRecDev -  возвращает номер записываемого устройства ( если оно есть, к прим. номер mic.rec ) \n
   @param recModeAvail - возвращает возможность записи устройства (  1 - есть, 0 - нету ) \n
   @param recModeStatus - возвращает состояние контролера ( в режиме записи 0 - нет, 1 - да ) \n
   @param ctrlMode - тип контролера ( -1 - не поддерживается, 0 - MONO, 1 - STEREO ) \n
   @param ctlStatus - возвращает состояние контролера ( 0 - выключено, 1 - включено ) \n
   @param L - уровень громкости левого канала ( для MONO устройств уровень громкости основного канала ) \n
   @param R - уровень громкости правого канала \n
   @param skipDev - игнорируемые устройства ( все записываемые, к прим. vol.rec, mic.rec, cd.rec, etc...) 0 - игнор, 1 - не игнор. \n
   @param enumListVariant - список доступных выриантов для ENUM контролеров ( к прим. Fast, Low, Medium или  48000, 44100, 32000, etc...) \n
   @param currentEnumName - название текущего варианта из enumListVariant \n
   @param currentEnumNum - номер текущего варианта из enumListVariant \n
   @param ctrlTypeName - тип контролера ( MIXT_ENUM, MIXT_MONOSLIDER, MIXT_STEREOSLIDER, MIXT_MONOSLIDER16, MIXT_STEREOPEAK, MIXT_ONOFF, etc.. ) \n

   Принимает номер контролера \n

   \code
   bool numRecDev = false;
   bool recModeAvail = false;
   bool recModeStatus = false;
   bool ctrlMode = false;
   bool ctlStatus = false;
   bool skipDev = false;
   int L = -1;
   int R = -1;
   int currentEnumNum = -1;
   std::string currentEnumName;
   int ctrlTypeName;
   std::string ctrlLabel;

   std::map < int, std::string > enumListVariant;

   std::map < int, std::string > MIX_DEV;
   ossmix -> returnListOfAvaibleControlDev ( MIX_DEV );

   for ( std::map < int, std::string > :: iterator it = MIX_DEV.begin(); it != MIX_DEV.end(); ++it ) {
      ossmix->getDevInfo ( it->second, control_num, numRecDev, recModeAvail,
      recModeStatus, SRC_MODE, ON, L, R, skipDev,  enumListVariant, currentEnumName, currentEnumNum, ctrlTypeName );

      if ( !skipDev ) {
         ....
      }
   }
   \endcode
*/
bool SilentMedia::SoundSystem::OSS::Mixer::getDevInfo (std::string                      param
      , std::string &                     ctrlLabel
      , int &                             ctrlNum
      , unsigned int &                    ctrlParent
      , int &                             numRecDev
      , bool &                            recModeAvail
      , bool &                            recModeStatus
      , short int &                       ctrlMode
      , short int &                       ctlStatus
      , int &                             L
      , int &                             R
      , int &                             M
      , int &                             minCtrlValue
      , int &                             maxCtrlValue
      , bool &                            skipDev
      , std::map < int, std::string > &   enumListVariant
      , std::string &                     currentEnumName
      , int &                             currentEnumNum
      , int &                             ctrlTypeName
      , int &                             ctrlFlag ) {
         ctrlNum = -1;
         ctrlParent = -1;
         /*
            Ставим в 0 а не в -1, потому, что есть устройства не имеющие возможности записи
            к примеру pcm, а -1 - значения ошибки
         */
         numRecDev = 0;
         recModeAvail = 0;
         recModeStatus = 0;
         ctrlMode = -1;
         ctlStatus = -1;
         L = -1;
         R = -1;
         M = -1;
         minCtrlValue = -1;
         maxCtrlValue = -1;
         ctrlFlag = -1;
         ctrlTypeName = -1;
         enumListVariant . clear();

                                     //Получаем имя устройства, без его класа ( все впроть до токи )
         ctrlLabel = param . substr ( param . find ( "." ) + 1, param . size() );

                                     //Такие устройства как vol.rec, mic.rec нужно пропускать
         skipDev = 0;
         std::string recmode = param + ".rec";

                                     //Получаем номер устройства исходя из его имени
         for ( std::map < int, std::string >::iterator it = listOfAvaibleCtrlDev . begin(); it != listOfAvaibleCtrlDev . end(); ++it ) {
            getNumByCtrlName ( param, ctrlNum );

                                        /*
            Проверяем существует ли устройство записи,
            если да то определяем находится ли он сейчас в режиме записи
                                        */
            if ( it -> second == recmode ) {
               numRecDev = it -> first;

                                           // Добавляем устройство в массив для пропуска
               listOfSkipCtrl [ numRecDev ] = it -> second;
               recModeAvail = 1;

               VAL ( numRecDev );

               ( val.value & 0xff ) ? recModeStatus = 1 : recModeStatus = 0;
            }
         }

                                     /*
         Проверяем есть ли устройство в миссиве, если да,
         то пропускаем его (skipDev = 1)
                                     */
         for ( std::map < int, std::string >::iterator it = listOfSkipCtrl . begin(); it != listOfSkipCtrl . end(); ++it ) {
            if ( it -> second == param ) {
               skipDev = 1;
// 				return true;
            }
         }

         /*
            Определяем включено ли устройство, если да,
            то определяем его значение громкости и режим
            ( srereo / mono)
         */
         getNumByCtrlName ( param, ctrlNum );
         VAL ( ctrlNum );
                              // оно нужно?!?
         val.value ? ctlStatus = 1 : ctlStatus = 0;

// 		std::string str_enumListVariantar;
// 		const char* ch_enumListVariantar;
               
// 		if ( ctlStatus ) {
         EXT ( ctrlNum );
         ctrlParent = ext . parent;
         minCtrlValue = ext . minvalue;
         maxCtrlValue = ext . maxvalue;
         ctrlFlag = ext . flags;
         ctrlTypeName = ext.type;
                        
// 			if ( ext . flags & MIXF_RECVOL ) {
// 				std::cout << ext . extname << std::endl;
// 			}

                        

         if ( ( ext.type == MIXT_MARKER ) || ( ext.type == MIXT_DEVROOT ) || ( ext.type == MIXT_GROUP ) || ( ext.type == MIXT_MONOPEAK ) ) {
            return 1;
         }

// MIXT_STEREOSLIDER: Stereo volume slider with 8 bit precision
// MIXT_STEREODB: Mixer control type
         if ( ( ext.type == MIXT_STEREOSLIDER ) || ( ext.type == MIXT_STEREODB ) ) {
            VAL ( ctrlNum );
            ctrlMode = 1;  // стерео-устройство
            L = ( val.value & 0xff );
            R = ( ( val.value >> 8 ) & 0xff );
         }

// MIXT_STEREOSLIDER16: Stereo volume slider with 16 bit precision
         if ( ext.type == MIXT_STEREOSLIDER16 ) {
            VAL ( ctrlNum );
            EXT ( ctrlNum );
            ctrlMode = 1;  // стерео-устройство
            L = ( val.value & 0xffff );
            R = ( ( val.value >> 16) & 0xffff );
                        // dB
            if ( ext.flags & MIXF_CENTIBEL ) {
               minCtrlValue = ext . minvalue / 10;
               maxCtrlValue = ext . maxvalue / 10;
               L = L / 10 + L % 10;
               R = R / 10 + R % 10;
            }
         }

// MIXT_MONOSLIDER16: Mono volume slider with 16 bit value field
         if ( ext.type == MIXT_MONOSLIDER16 ) {
            VAL ( ctrlNum );
            EXT ( ctrlNum );
            ctrlMode = 0;  // моно-устройство
            M = ( val.value & 0xffff );
                        // dB
            if ( ext.flags & MIXF_CENTIBEL ) {
               minCtrlValue = ext . minvalue / 10;
               maxCtrlValue = ext . maxvalue / 10;
               M = M / 10;
            }
         }

// 						case MIXT_3D:				// Stereo volume slider with 16 bit precision
// 						VAL ( ctrlNum );
// 							ctrlMode = 1;
         // 						
// 							X = ( (val.value >> 8) & 0xff );
// 							Y = ( val.value & 0x00ff );
// 							Z = ( (val.value >> 16) & 0xffff );
// 							break;

// Показывает уровень выходного и входного сигналов
// MIXT_STEREOVU:          Mixer control type
// MIXT_STEREOPEAK:        Stereo peak meter control
         if ( ( ext.type == MIXT_STEREOVU ) || ( ext.type == MIXT_STEREOPEAK ) ) {
            VAL ( ctrlNum );
            L = ( val.value & 0xff );
            R = ( ( val.value >> 8 ) & 0xff );
         }

// MIXT_ENUM:              Enumerated mixer control
         if ( ext.type == MIXT_ENUM ) {
            VAL ( ctrlNum );
            EXT ( ctrlNum );
            EI ( ctrlNum );

            enumListVariant . clear();
            for ( int i = 0; i < ei.nvalues; i++ ) {
               if (ext.enum_present[i / 8] & (1 << (i % 8))) {
                  enumListVariant [ i ] = ei.strings + ei.strindex [ i ];
               }
            }
            currentEnumNum = val.value & 0xff; // Номер текущего елемента
            currentEnumName = enumListVariant [ currentEnumNum ]; //  Имя текущего элемента
         }


// MIXT_MONOSLIDER:        8 bit mono volume slider
// MIXT_MONODB:            Mixer control type (obsolete)
         if ( ( ext.type == MIXT_MONOSLIDER ) || ( ext.type ==  MIXT_MONODB ) ) {
            VAL ( ctrlNum );

            ctrlMode = 0;
            M = ( val.value & 0xff );
         }

//  MIXT_SLIDER:           Mono volume slider with expanded range
         if ( ext.type == MIXT_SLIDER ) {
            VAL ( ctrlNum );
            ctrlMode = 0;
            M = val.value;
         }

// MIXT_MONOVU:            Undefined control
         if ( ext.type == MIXT_MONOVU ) {
            VAL ( ctrlNum );
            ctrlMode = 1;
            M = ( val.value & 0xff );
         }

// MIXT_VALUE:             General purpose decimal value mixer control
         if ( ext.type == MIXT_VALUE ) {
            VAL ( ctrlNum );
            ctrlMode = 0;
            M = val.value;
         }

// MIXT_HEXVALUE:          Hexadecimal mixer value control
         if ( ext.type == MIXT_HEXVALUE ) {
            VAL ( ctrlNum );
            ctrlMode = 0;
            M = val.value;
         }

// MIXT_ONOFF:             Hexadecimal mixer value control
         if ( ext.type == MIXT_ONOFF ) {
            VAL ( ctrlNum );
            if ( val.value ) {
               currentEnumName = "ON";
               currentEnumNum = 1;
            } else {
               currentEnumName = "OFF";
               currentEnumNum = 0;
            }
         }
         return true;
}

void SilentMedia::SoundSystem::OSS::Mixer::getPeak ( int ctrlNum, int &L, int &R ) {
   VAL ( ctrlNum );
   L = ( val.value & 0xff );
   R = ( ( val.value >> 8 ) & 0xff );
}

/// Установка значения громкости
/**
      Принимает номер контролера, значения левого и правого каналов. \n
      Для моно устройств R = -1. \n
      Канал значения которого мы не хочем менять, ставим в -1 \n

      \code
      int ctrlNum = 2; // номер нужного нам контролера
      ssmix -> SS_DevSetParam ( ctrlNum, -1, 80 );
      \endcode

      приведет к установке правого канала в значение громкости 80, а левый оставит на прежнем уровне. \n

      @param ctrlNum - номер контролера
      @param L - значение громкости левого канала
      @param R - значение громкости правого канала

*/
bool SilentMedia::SoundSystem::OSS::Mixer::setDevVol ( int ctrlNum, int L, int R, int M )  {
   vr.dev = default_mixer_dev;
   vr.ctrl = ctrlNum;
   vr.timestamp = ext.timestamp;

   VAL ( ctrlNum );
   EXT ( ctrlNum );

// Установка левого канала, правый не трогаем
   if ( R == -1 && L && M == -1 ) {
      if (
          ( ext.type == MIXT_STEREOSLIDER ) || ( ext.type == MIXT_STEREODB ) || ( ext.type == MIXT_MONOSLIDER ) 
          || ( ext.type == MIXT_MONODB ) || ( ext.type == MIXT_MONOVU ) || ( ext.type == MIXT_VALUE ) 
          || ( ext.type == MIXT_SLIDER ) || ( ext.type == MIXT_HEXVALUE ) || ( ext.type == MIXT_HEXVALUE ) 
         ) {
         vr.value = ( ( L & 0x00ff ) | ( ( ( ( val.value >> 8 ) & 0xff ) & 0x00ff ) << 8 ) );
         } else if ( ( ext.type == MIXT_STEREOSLIDER16 ) || ( ext.type == MIXT_MONOSLIDER16 ) ) {
            vr.value = ( ( L*10 & 0x00ff ) | ( ( ( ( val.value >> 16) & 0xffff ) & 0x00ff ) << 16 ) );
         }

   // Установка правого канала, левый не трогаем
   } else if ( L == -1 && R && M == -1 ) {
      if (
          ( ext.type == MIXT_STEREOSLIDER ) || ( ext.type == MIXT_STEREODB ) || ( ext.type == MIXT_MONOSLIDER ) 
          || ( ext.type == MIXT_MONODB ) || ( ext.type == MIXT_MONOVU ) || ( ext.type == MIXT_VALUE ) 
          || ( ext.type == MIXT_SLIDER ) || ( ext.type == MIXT_HEXVALUE ) || ( ext.type == MIXT_HEXVALUE ) 
         ) {
         vr.value = ( ( ( val.value & 0xff ) & 0xff ) | ( ( R & 0x00ff ) << 8 ) );
         } else if ( ext.type == MIXT_STEREOSLIDER16 ) {
            // vr.value = ( ( L*10 & 0x00ff ) | ( ( ( ( val.value >> 16) & 0xffff ) & 0x00ff ) << 16 ) );
            vr.value = ( ( ( val.value & 0xff ) & 0xff ) | ( ( R*10 & 0x00ff ) << 16 ) );
         }

   // Установка левого и правого каналов
   } else if ( L && R && M == -1 ) {
      if (
          ( ext.type == MIXT_STEREOSLIDER ) || ( ext.type == MIXT_STEREODB ) || ( ext.type == MIXT_MONOSLIDER ) 
          || ( ext.type == MIXT_MONODB ) || ( ext.type == MIXT_MONOVU ) || ( ext.type == MIXT_VALUE ) 
          || ( ext.type == MIXT_SLIDER ) || ( ext.type == MIXT_HEXVALUE ) || ( ext.type == MIXT_HEXVALUE ) 
         ) {
         vr.value = ( ( L & 0x00ff ) | ( R & 0x00ff ) << 8 );
         } else if ( ext.type == MIXT_STEREOSLIDER16 ) {
            vr.value = ( ( ( val.value & 0xff ) & 0xff ) | ( ( R*10 & 0x00ff ) << 16 ) );
         }
                                                      // Установка моно канала
   } else if ( M && L == -1 && R == -1 ) {
      if ( ( ext.type == MIXT_MONOSLIDER ) || ( ext.type == MIXT_MONODB ) || ( ext.type == MIXT_MONOVU ) ) {
         vr.value = ( ( M & 0xff ) & 0xff );
      } else if ( ext.type == MIXT_MONOSLIDER16 ) {
         vr.value = ( M*10 & 0x00ff );
      }
   }

   if ( ioctl ( mixer_fd, SNDCTL_MIX_WRITE, &vr ) == -1 ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         return ( _debug ( "SNDCTL_MIX_WRITE" ) );
      }
   }
   return true;
}

bool SilentMedia::SoundSystem::OSS::Mixer::setDevVol ( std::string ctrlName, int L, int R, int M )  {
   int ctrlNum = -1;
   getNumByCtrlName ( ctrlName, ctrlNum );
   setDevVol ( ctrlNum, L, R, M );
   return true;
}

/// Включение/выключение устройства
/**
      @param ctrlNum  номер контролера
      @param ON 1 - включить, 0 - выключить
      @param L,R  1 - применить действие к данному каналу

      \code
      int ctrlNum = 2;
      // Выключить левый канал
      ssmix -> SS_DevSetParam ( ctrlNum, 0, 1, 0 );
      // Выключить правый канал
      ssmix -> SS_DevSetParam ( ctrlNum, 0, 0, 1 );
      // Выключить оба канала
      ssmix -> SS_DevSetParam ( ctrlNum, 0, 1, 1 );
      // Включить оба канала
      ssmix -> SS_DevSetParam ( ctrlNum, 1, 1, 1 );
      \endcode

      Заметьте, что при влючении каналов, восстанавливаются предедущие сохраненные значения громкостей \n
*/
bool SilentMedia::SoundSystem::OSS::Mixer::onOffDev ( int ctrlNum, bool ON, bool L, bool R, bool M )  {
   // Параметры для взятия значения текущего значения громкости контролера
   VAL ( ctrlNum );

   // Параметры для записи значения громкости контролера
   vr.dev = default_mixer_dev;
   vr.ctrl = ctrlNum;
   vr.timestamp = ext.timestamp;

   /*
      Если выключаем контролер, предварительно записываем текущие значения
      громкости, и ставим громкость в 0
   */
   if ( !ON ) {
               // Если выключам левый канал
      if ( L & !R ) {
         devNumValL [ ctrlNum ] = ( val.value & 0xff );
         vr.value = ( 0 | ( ( ( ( val.value >> 8 ) & 0xff ) & 0x00ff ) << 8 ) );
      }
      // Если выключаем правый канал
      else if ( R & !L ) {
         devNumValR [ ctrlNum ] = ( ( val.value >> 8 ) & 0xff );
         vr.value = ( ( ( val.value & 0xff ) & 0xff ) | 0 );
      // Если выключаем оба канала
      } else if ( L & R ) {
         devNumValL [ ctrlNum ] = ( val.value & 0xff );
         devNumValR [ ctrlNum ] = ( ( val.value >> 8 ) & 0xff );
         vr.value = ( 0 | 0 );
      } else if ( M ) {
         devNumValM [ ctrlNum ] = ( val.value & 0xff );
         vr.value = ( 0 );
      }
   // Если ON = 1 то восстанавливаем значение громкостей каналов
   } else if  ( ON ) {
      // Если включам левый канал
      if ( L & !R ) {
         vr.value = ( ( ( devNumValL [ ctrlNum ] ) & 0x00ff ) | ( ( ( ( val.value >> 8 ) & 0xff ) & 0x00ff ) << 8 ) );
      }
      else if ( R & !L ) {
         vr.value = ( ( ( val.value & 0xff ) & 0xff ) | ( ( ( devNumValR [ ctrlNum ] ) & 0x00ff ) << 8 ) );
      }
      else if ( R & L ) {
         vr.value = ( ( ( devNumValL [ ctrlNum ] ) & 0x00ff ) | ( ( devNumValR [ ctrlNum ] ) & 0x00ff ) << 8 );
      } else if ( M ) {
         vr.value = ( devNumValM [ ctrlNum ] & 0x00ff );
      }
   }

   if ( ioctl ( mixer_fd, SNDCTL_MIX_WRITE, &vr ) == -1 ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         return ( _debug ( "SNDCTL_MIX_WRITE" ) );
      }
   }
   return true;
}

/// Меняем значение/состояния контролеров ( ON/OFF, ENUM )
/**
      @param ctrlNum номер меняемого контролера
      @param state состояние на которое меняем ( 0, 1 - для ON/OFF, 0 - ... - для ENUM )

      на пример

      \code
      int ctrlNum = 38; // номер нужного контролера
      ssmix -> changeDevState ( ctrlNum, 5 )
      \endcode

      выставит vmix0-src в Production. \n
      Возможные значения можно получить через
      @see SilentMedia::SoundSystem::OSS::Mixer::getDevInfo() \n
      список std::map < int, std::string > enumListVariant
*/
bool SilentMedia::SoundSystem::OSS::Mixer::changeDevState ( int ctrlNum, int state )  {
   vr.value = state;
   VR ( ctrlNum );
   return true;
}

bool SilentMedia::SoundSystem::OSS::Mixer::changeDevState ( std::string ctrlName, int state )  {
   int ctrlNum = -1;
   getNumByCtrlName ( ctrlName, ctrlNum );
   changeDevState ( ctrlNum, state );
   return true;
}

bool SilentMedia::SoundSystem::OSS::Mixer::changeDevState ( int ctrlNum, std::map < int, std::string > enumListVariant, std::string state )  {
   for ( std::map < int, std::string >::iterator it = enumListVariant . begin(); it != enumListVariant . end(); ++it ) {
      if ( it -> second == state ) {
         vr.value = it -> first;
         VR ( ctrlNum );
         return true;
      }
   }
   return false;
}

bool SilentMedia::SoundSystem::OSS::Mixer::changeDevState ( std::string ctrlName, std::map < int, std::string > enumListVariant, std::string state )  {
   int ctrlNum = -1;
   getNumByCtrlName ( ctrlName, ctrlNum );
   changeDevState ( ctrlNum, enumListVariant, state );
   return true;
}

// ---------------------------------------------------------------------------
//  Private methods
// ---------------------------------------------------------------------------

/**
   @return Количество контролеров в системе \n
 */
bool SilentMedia::SoundSystem::OSS::Mixer::_OSS_NREXT ( void ) {
   nrext = default_mixer_dev; // default_mixer_dev = 0
   if ( ioctl ( mixer_fd, SNDCTL_MIX_NREXT, &nrext ) == -1 ) { // nrext = 72
      if ( SILENTMEDIA_DEBUG == true ) {
         return ( _debug ( "SNDCTL_MIX_NREXT" ) );
      }
   }
   return true;
}

/**
   Составляет список все доступных контролеров \n
   Заполняет std::map значениеми: \n
   номер контролера => имя контролера \n
   Доступ к нему можно получит через void SilentMedia::SoundSystem::OSS::Mixer::returnListOfAvaibleControlDev \n
 */
bool SilentMedia::SoundSystem::OSS::Mixer::getControlList ( void ) {
// tmpFullAvaibleMixDev.clear();
   for ( int i = 1; i < nrext; i++ ) {
      unsigned short int ctrlNum = i;
      this -> EXT ( ctrlNum );

// std::cout << ext.extname << " : " << ext.update_counter << std::endl;

      if ( ext.type != MIXT_MARKER ) {
// std::cout << ext.extname << " : " << ext.parent << " : " << ext.ctrl << std::endl;
         parentList [ i ] = ext.parent;
         if ( ext.parent == 0 ) {
            fullCtrlList [ i ] = ext.extname;
         }
      }

      if ( ext.flags & MIXF_READABLE ) {
         listOfReadableCtrlDev [ i ] = ext.extname;
      }

      if ( ext.flags & MIXF_WRITEABLE ) {
         listOfWriteableCtrlDev [ i ] = ext.extname;
      }

      if ( ext.type != MIXT_DEVROOT && ext.type != MIXT_GROUP && ext.type != MIXT_MARKER ) {
// std::cout << ext.extname << " : " << ext.parent << " : " << ext.ctrl << std::endl;
         listOfAvaibleCtrlDev [ i ] = ext.extname;
      }
   }
   return true;
}

bool SilentMedia::SoundSystem::OSS::Mixer::VAL ( int ctrlNum ) {
   val.dev = default_mixer_dev;
   val.ctrl = ctrlNum;
   val.timestamp = ext.timestamp;
   if ( ioctl ( mixer_fd, SNDCTL_MIX_READ, &val ) == -1 ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         std::cout << "DEBUG INFO: " << "default_mixer_dev = " << default_mixer_dev << std::endl;
         std::cout << "DEBUG INFO: " << "ctrlNum = " << ctrlNum << std::endl;
         return ( _debug ( "SNDCTL_MIX_READ" ) );
      }
   }
   return true;
}

bool SilentMedia::SoundSystem::OSS::Mixer::VR ( int ctrlNum ) {
   vr.dev = default_mixer_dev;
   vr.ctrl = ctrlNum;
   vr.timestamp = ext.timestamp;
   if ( ioctl ( mixer_fd, SNDCTL_MIX_WRITE, &vr ) == -1 ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         std::cout << "DEBUG INFO: " << "default_mixer_dev = " << default_mixer_dev << std::endl;
         std::cout << "DEBUG INFO: " << "ctrlNum = " << ctrlNum << std::endl;
         return ( _debug ( "SNDCTL_MIX_WRITE" ) );
      }
   }
   return true;
}

bool SilentMedia::SoundSystem::OSS::Mixer::MI ( void ) {
   mi.dev = default_mixer_dev;
   if ( ioctl ( mixer_fd, SNDCTL_MIXERINFO, &mi ) == -1 ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         return ( _debug ( "SNDCTL_MIXERINFO" ) );
      }
   }
   return true;
}

bool SilentMedia::SoundSystem::OSS::Mixer::EXT ( int ctrlNum ) {
   ext.dev = default_mixer_dev;
   ext.ctrl = ctrlNum;
   if ( ioctl ( mixer_fd, SNDCTL_MIX_EXTINFO, &ext ) == -1 ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         std::cout << "DEBUG INFO: " << "default_mixer_dev = " << default_mixer_dev << std::endl;
         std::cout << "DEBUG INFO: " << "ctrlNum = " << ctrlNum << std::endl;
         return ( _debug ( "SNDCTL_MIX_EXTINFO" ) );
      }
   }
   return true;
}

bool SilentMedia::SoundSystem::OSS::Mixer::EI ( int ctrlNum ) {
   ei.dev = default_mixer_dev;
   ei.ctrl = ctrlNum;
   if ( ioctl ( mixer_fd, SNDCTL_MIX_ENUMINFO, &ei ) == -1 ) {
      if ( SILENTMEDIA_DEBUG == true ) {
         std::cout << "DEBUG INFO: " << "default_mixer_dev = " << default_mixer_dev << std::endl;
         std::cout << "DEBUG INFO: " << "ctrlNum = " << ctrlNum << std::endl;
         return ( _debug ( "SNDCTL_MIX_ENUMINFO" ) );
      }
   }
   return true;
}

/**
Находим дефолтный микшер в системе ( 0 к примеру ) \n
Нужна для того, если в системе установлено больше одной звуковой карты. \n
Внутренняя ф-ция. Не нуждается в явном применении. \n
@return Номер микшера по умолчанию. \n
*/

int SilentMedia::SoundSystem::OSS::Mixer::findDefaultMixerDev ( void ) {
   _OSS_SNDCTL_SYSINFO();
   int best_pri = -2;
   for ( int i = 0; i < sysinfo.nummixers; i++ ) {
      mi.dev = i;
      if ( mi.priority < -1 )
         continue;
      if ( mi.priority > best_pri ) {
         default_mixer_dev = i;
         best_pri = mi.priority;
      }
   }
   return default_mixer_dev;
}

/**
@return Системную информацию OSS \n
*/

bool SilentMedia::SoundSystem::OSS::Mixer::_OSS_SNDCTL_SYSINFO ( void ) {
   if ( ioctl ( mixer_fd, SNDCTL_SYSINFO, &sysinfo ) == -1 ) {
      return ( _debug ( "SNDCTL_SYSINFO" ) );
   }
   return false;
}
