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

#include <FLAC++/all.h>
#include "Audio.hpp"

#include <SML/Audio/Codec/DecodedData.hpp>
#include <SML/Audio/SoundSystem/SoundSystemManager.hpp>
#include <SML/Audio/Codec/WAV/WAV.hpp>
// #include <SML/Audio/Codec/FLAC/FLAC.hpp>
// #include <SML/Audio/Codec/Vorbis/Vorbis.hpp>
// #include <SML/Audio/Codec/WavPack/WavPack.hpp>

void * playThreadFunc ( void * data ) {
//    static_cast < SilentMedia::Audio * > ( data ) -> play_();
   SilentMedia::Audio * audio = SilentMedia::Audio::Instance();
   std::string id = static_cast < const char * > ( data );
   audio -> play_ ( id );
   return NULL;
}

SilentMedia::Audio * SilentMedia::Audio ::_audio = NULL;

SilentMedia::Audio::Audio ( void ) : audio ( NULL ), ext ( "wav" ), songStatus ( Stop ),
                            engineType ( _Game )
{
   srand ( time ( NULL ) );
   this -> ssystem = SoundSystemManager::Instance();
   this -> ddata = DecodedData::Instance();
}

SilentMedia::Audio::~Audio ( void ) {
//    std::cout << "Audio Destructor.. " << std::endl;
   
   // Маленький хак. Если программист забудет вызвать delete, но деструктор не запустится,
   // а значит и не воспроизвдеться песня :)))
//    pthread_join ( this -> threadMap [ this -> objId ], NULL );

   delete this -> ddata; this -> ddata = NULL;
   delete this -> ssystem; this -> ssystem = NULL;

   delete this -> objs [ "wav" ]; this -> objs [ "wav" ] = NULL;
   delete this -> objs [ "flac" ]; this -> objs [ "flac" ] = NULL;
   delete this -> objs [ "ogg" ]; this -> objs [ "ogg" ] = NULL;
   delete this -> objs [ "wv" ]; this -> objs [ "wv" ] = NULL;
}

SilentMedia::Audio * SilentMedia::Audio::Instance ( void ) {
   if ( _audio == NULL ) {
         _audio = new Audio();
   }
   return ( _audio );
}

SilentMedia::AbstractCodec * SilentMedia::Audio::getNeedObj ( void ) {
   if ( this -> ext == "wav" ) {
      return ( new Codec::WAV() );
   }/* else if ( this -> ext == "flac" ) {
      return ( new Codec::FLAC() );
   } else if ( this -> ext == "vorbis" ) {
      return ( new Codec::Vorbis() );
   } else if ( this -> ext == "wavpack" ) {
      return ( new Codec::WavPack() );
   }*/
}

bool SilentMedia::Audio::init ( EngineType engineType, AudioComponents component, SoundSystemType soundSystem, std::string dspDev, std::string mixerDev ) {
//    this -> ssystem -> setSoundSystem ( soundSystem );
//    this -> ssystem -> setDSPDev ( dspDev );
   
   this -> engineType = engineType;
//    this -> soundSystemObj ( soundSystem, dspDev, mixerDev );
//    if ( ( soundSystem . empty() ) || ( soundSystem != "OSS" || soundSystem != "ALSA" ) ) {
//       std::cout << "Error in SilentMedia::Audio::init(): error 1-st param, please specific Sound System!" << std::endl;
//    } else if ( soundSystem == "OSS" ) {
//       this -> oss = new OSS();
//       this -> dsp = new DSP ( dspDev );
//       this -> mixer = new Mixer ( mixerDev );
//    }
//    if ( dspDev . empty() ) {
//       std::cout << "Error in SilentMedia::Audio::init(): please specific dspDev param!" << std::endl;
//    } else {
//    }

//    _ssystem = new SoundSystem;
//    std::cout << "Mu dev: " << dspDev << " : " << mixerDev << std::endl;
//    std::cout << ssystem << std::endl;
//    if ( soundSystem & OSS ) {
//       if ( component & DSP ) {
   return ( this -> ssystem -> init ( component, soundSystem, dspDev, mixerDev ) );

//    this -> ssystem -> init ( component, "OSS", "/dev/dsp", "/dev/mixer" );

//    _ddata = new DecodedData;

//    _dsp -> openFile ( "/data/music/1/Ein Lied.wav" );
   
//    _dsp -> changeStatus ( Play );
   
//    return true;
}

// bool SilentMedia::Audio::initMultiPlay ( void ) {
//    return ( this -> ssystem -> initMultiPlay() );
// }

bool SilentMedia::Audio::openFile ( std::string inputFile, std::string& id ) {
//    this -> ddata -> clean(); // при каждой инициализации нового файла очищаем данные
//    this -> inputFile = inputFile;
//    this -> ext = inputFile . substr ( ( ( inputFile . find (".", inputFile . size() - 5 ) ) + 1 ), inputFile . size() );
//    return ( objs [ this -> ext ] -> init ( this -> inputFile ) );
//    return true;
   
   this -> ddata -> clean(); // при каждой инициализации нового файла очищаем данные
   this -> inputFile = inputFile;
   this -> ext = this -> inputFile . substr ( ( ( this -> inputFile . find (".", this -> inputFile . size() - 5 ) ) + 1 ), this -> inputFile . size() );
   
// std::cout << "id: " << id << std::endl;

   // Проверяем уникальный ли идентификатор,если нет то прибавляем к идентификатору случайное число
   // Нужно чтобы в один момент времени могли воспроизводится файлы с одинаковыми идентификаторами
   if ( this -> idList . find ( id ) == this -> idList.end() ) { // если уникален
      this -> idList . insert ( id );
//       objId = id;
   } else {
      std::ostringstream num;
      num << rand();
      std::string modValue = id + num.str();
      this -> idList . insert ( modValue );
      id = modValue;
   }

//    std::stringstream objIds;
//    objIds << std::hex << idObjs [ id ] << std::dec;
//    std::string objId = objIds.str();
//    this -> objId = id;

//    std::cout << "ID: " << id << std::endl;
   
   idObjs [ id ] = this -> getNeedObj();
   return ( idObjs [ id ] -> init ( this -> inputFile, id ) );
}

// Возможно надо будет inputFile сделать локальным
bool SilentMedia::Audio::play ( std::string inputFile, std::string id, std::string next,
                                int cycleCount, unsigned int pauseDelay )
{
   this -> openFile ( inputFile, id );

//    playThreadFunc(this);
//    this -> threadMap [ objId ];
//    std::cout << "PTHREAD_START: " << id << std::endl;
// FIXME: Не могу через pthread_create передать нормальный std::string, приходится передавать const char *
//    playThreadFunc ( ( void * ) id.c_str() );
   pthread_create ( &this -> threadMap [ id ], NULL, playThreadFunc, ( void * ) id.c_str() );

   return true;
}

bool SilentMedia::Audio::destroyObj ( std::string id ) {
//    std::cout << "In Audio::destroyObj()" << std::endl;
//    std::cout << "Audio::destroyObj: id = " << id << std::endl;
   delete idObjs [ id ]; idObjs [ id ] = NULL;
//    this -> idList . erase ( id );
//    std::cout << "PTHREAD_STOP: " << id << std::endl;
   if ( threadMap [ id ] ) {
      pthread_join ( this -> threadMap [ id ], NULL );
   }
}

bool SilentMedia::Audio::changeStatus ( Status setStatus ) {
   if ( setStatus == Play ) {
      this -> playExt = this -> ext;
//       if ( this -> songStatus == Pause ) {
//       }
      objs [ this -> ext ] -> flush();

      if ( this -> songStatus == Play ) {
         pthread_cancel ( thread1 );
      }

     pthread_create ( &thread1, NULL, playThreadFunc, this );
//       play_();

      this -> songStatus = setStatus;
   } else if ( setStatus == Stop ) {
      objs [ this -> ext ] -> flush();

      if ( this -> songStatus == Play ) {
         pthread_cancel(thread1);
      }
      this -> songStatus = setStatus;
   } else if ( setStatus == Pause ) {
      if ( this -> songStatus == Play ) {
         pthread_cancel(thread1);
      }
      this -> songStatus = setStatus;
   } else if ( setStatus == Resume ) {
      pthread_create ( &thread1, NULL, playThreadFunc, this );
      this -> songStatus = Play;
   }
   return true;
}

void SilentMedia::Audio::play_ ( std::string id ) {
//    this -> ssystem -> suspend();

   if ( this -> engineType == _Game ) {
      idObjs [ id ] -> play ( id );
   } else {
   
//    this -> songStatus = Play;
//    objs [ this -> ext ] -> play();
   }
}

void SilentMedia::Audio::closeF ( std::string id ) {
   this -> idObjs [ id ] -> closeF();
}

void SilentMedia::Audio::closeF ( void ) {
   objs [ this -> ext ] -> closeF();
}

void SilentMedia::Audio::flush ( void ) {
   objs [ this -> ext ] -> flush();
}

void SilentMedia::Audio::finish ( void ) {
   objs [ this -> ext ] -> finish();
}

void SilentMedia::Audio::setSeek ( int val ) {
   pthread_cancel ( thread1 );
   objs [ this -> ext ] -> setSeek ( val );
   pthread_create ( &thread1, NULL, playThreadFunc, this );
}

double SilentMedia::Audio::getSeek ( void ) {
   /*
   выбираем не this -> ext, а this -> playExt, так как при переключении с песни на песню нам нужно продолжать брать позицию с текущей играющей песни
   - this -> playExt. Иначе если взять с this -> ext то юудет скакать ползунок при переключении с песни на песню ( в случае разных форматов ).
   Обязательно проверяем не пустое ли значение this -> playExt, иначе - Ошибка сегментирования
      */
   if ( ( this -> playExt ) . empty() ) {
      return 0;
   } else {
      return ( objs [ this -> playExt ] -> getSeek() );
   }
}
