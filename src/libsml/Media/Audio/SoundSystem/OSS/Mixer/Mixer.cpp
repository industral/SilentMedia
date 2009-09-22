/*******************************************************************************
 * Copyright (c) 2009, Alex Ivasyuv                                            *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *                                                                             *
 * 1. Redistributions of source code must retain the above copyright           *
 *    notice, this list of conditions and the following disclaimer.            *
 * 2. Redistributions in binary form must reproduce the above copyright        *
 *    notice, this list of conditions and the following disclaimer in the      *
 *    documentation and/or other materials provided with the distribution.     *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY Alex Ivasyuv ''AS IS'' AND ANY                 *
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED   *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE      *
 * DISCLAIMED. IN NO EVENT SHALL Alex Ivasyuv BE LIABLE FOR ANY DIRECT,        *
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES          *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;*
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF    *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           *
 ******************************************************************************/

// API http://manuals.opensound.com/developer/MIXER.html

#include "Mixer.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace OSS {
          namespace Mixer {

            // --------------------------------------------------------------------
            // Public methods
            // --------------------------------------------------------------------

            Mixer::Mixer() :
              default_mixer_dev(-1), mixer_fd(-1), numberOfControls(-1) {
            }

            Mixer::Mixer(const string dev) {
              this -> init(dev);
            }

            Mixer::~Mixer() {
            }

            //TODO: move this var
            bool Mixer::init() {
              return (this -> init("/dev/mixer"));
            }

            bool Mixer::init(const string dev) {
              if ((this -> mixer_fd = open(dev.c_str(), O_RDWR)) == -1) {
                cerr << "Can't open mixer device: " << dev << endl;
                return false;
              } else {
                if (this -> getMixerInfo() == false) {
                  return false;
                } else if (this -> getSysInfo() == false) {
                  return false;
                } else {
                  this -> findDefaultMixerDev();
                }

                if (this -> getNumberOfControls() == false) {
                  return false;
                } else if (this -> getControlList() == false) {
                  return false;
                }
              }
              return true;
            }

            int Mixer::getUpdateCounter() {
              this -> getMixerInfo();
              return mi.modify_counter;
            }

            map<int, string> Mixer::getListOfCtrl() const {
              return this -> ctrlList;
            }

            int Mixer::getCtrlNumberByName(const string controlName) {
              int ctrlNum = -1;
              for (map<int, string>::iterator it = this -> fullCtrlList.begin(); it
                  != this -> fullCtrlList.end(); ++it) {
                if (it -> second == controlName) {
                  ctrlNum = it -> first;
                  return ctrlNum;
                }
              }
              return ctrlNum;
            }

            bool Mixer::checkRecAvail(const string ctrlName) {
              const string str = this -> getRecCtrlName(ctrlName);

              for (map<int, string>::iterator it = this -> fullCtrlList.begin(); it
                  != this -> fullCtrlList.end(); ++it) {
                if (it->second == str) {
                  return true;
                }
              }
              return false;
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
             string currentEnumName;
             int ctrlTypeName;
             string ctrlLabel;

             map < int, string > enumListVariant;

             map < int, string > MIX_DEV;
             ossmix -> getListOfCtrl ( MIX_DEV );

             for ( map < int, string > :: iterator it = MIX_DEV.begin(); it != MIX_DEV.end(); ++it ) {
             ossmix->getCtrlInfo ( it->second, control_num, numRecDev, recModeAvail,
             recModeStatus, SRC_MODE, ON, L, R, skipDev,  enumListVariant, currentEnumName, currentEnumNum, ctrlTypeName );

             if ( !skipDev ) {
             ....
             }
             }
             \endcode
             */
            bool Mixer::getCtrlInfo(const int ctrlNum, string & ctrlLabel,
                int & numRecDev, bool & recModeAvail, bool & recModeStatus,
                bool & stereo, bool & on, int & L, int & R, int & M,
                int & minCtrlValue, int & maxCtrlValue,
                map<int, string> & enumListVariant, string & currentEnumName,
                int & currentEnumNum, int & ctrlTypeName, int & ctrlFlag) {

              /*
               * Set numRecDev to 0 instead of -1, as there present controllers
               * that hasn't record capabilities. They will has -1.
               */
              numRecDev = 0;
              recModeAvail = false;
              recModeStatus = false;
              stereo = false;
              on = false;
              L = -1;
              R = -1;
              M = -1;
              minCtrlValue = -1;
              maxCtrlValue = -1;
              ctrlFlag = -1;
              ctrlTypeName = -1;
              enumListVariant.clear();

              // get controller name
              const string ctrlName = this -> ctrlList[ctrlNum];

              // get controller name, without it class (up to dot)
              ctrlLabel = ctrlName.substr(ctrlName.find(".") + 1,
                  ctrlName.size());

              /* Check if controller has record capabilities */
              // get record controller name
              const string recCtrlName = this -> getRecCtrlName(ctrlName);

              // get record controller number
              const int recCtrlNumber =
                  this -> getCtrlNumberByName(recCtrlName);

              for (list<int>::iterator it = this -> recCtrlList.begin(); it
                  != this -> recCtrlList.end(); ++it) {
                const int foundRecCtrlNumber = *it;
                if (foundRecCtrlNumber == recCtrlNumber) {
                  recModeAvail = true;

                  VAL(foundRecCtrlNumber);
                  (val.value & 0xff) ? recModeStatus = true : recModeStatus
                      = false;
                }
              }

              // get controller capabilities
              this -> VAL(ctrlNum);
              this -> getExtensionInfo(ctrlNum);

              // set on/off based on value
              val.value ? on = true : on = false;

              minCtrlValue = ext.minvalue;
              maxCtrlValue = ext.maxvalue;
              ctrlFlag = ext.flags;
              ctrlTypeName = ext.type;

              // MIXT_STEREOSLIDER: Stereo volume slider with 8 bit precision
              if (ext.type == MIXT_STEREOSLIDER) {
                stereo = true;
                L = (val.value & 0xff);
                R = ((val.value >> 8) & 0xff);
                // MIXT_STEREOSLIDER16: Stereo volume slider with 16 bit precision
              } else if (ext.type == MIXT_STEREOSLIDER16) {
                stereo = true;
                L = (val.value & 0xffff);
                R = ((val.value >> 16) & 0xffff);
                // dB
                if (ext.flags & MIXF_CENTIBEL) {
                  minCtrlValue = ext.minvalue / 10;
                  maxCtrlValue = ext.maxvalue / 10;
                  L = L / 10 + L % 10;
                  R = R / 10 + R % 10;
                }
                // MIXT_MONOSLIDER16: Mono volume slider with 16 bit value field
              } else if (ext.type == MIXT_MONOSLIDER16) {
                M = (val.value & 0xffff);
                // dB
                if (ext.flags & MIXF_CENTIBEL) {
                  minCtrlValue = ext.minvalue / 10;
                  maxCtrlValue = ext.maxvalue / 10;
                  M = M / 10;
                }
                // show input/output level
                // MIXT_STEREOVU:   Mixer control type
                // MIXT_STEREOPEAK: Stereo peak meter control
              } else if ((ext.type == MIXT_STEREOVU) || (ext.type
                  == MIXT_STEREOPEAK)) {
                L = (val.value & 0xff);
                R = ((val.value >> 8) & 0xff);
                // MIXT_ENUM: Enumerated mixer control
              } else if (ext.type == MIXT_ENUM) {
                EI(ctrlNum);

                enumListVariant.clear();
                for (int i = 0; i < ei.nvalues; i++) {
                  if (ext.enum_present[i / 8] & (1 << (i % 8))) {
                    enumListVariant[i] = ei.strings + ei.strindex[i];
                  }
                }
                currentEnumNum = val.value & 0xff; // Номер текущего елемента
                currentEnumName = enumListVariant[currentEnumNum]; //  Имя текущего элемента
                // MIXT_MONOSLIDER: 8 bit mono volume slider
                // MIXT_MONOVU:     Undefined control
              } else if ((ext.type == MIXT_MONOSLIDER) || (ext.type
                  == MIXT_MONOVU)) {
                M = (val.value & 0xff);
                // MIXT_SLIDER:   Mono volume slider with expanded range
                // MIXT_VALUE:    General purpose decimal value mixer control
                // MIXT_HEXVALUE: Hexadecimal mixer value control
              } else if ((ext.type == MIXT_SLIDER) || (ext.type == MIXT_VALUE)
                  || (ext.type == MIXT_HEXVALUE)) {
                M = val.value;
                // MIXT_ONOFF: Hexadecimal mixer value control
              } else if (ext.type == MIXT_ONOFF) {
                if (val.value) {
                  currentEnumName = "ON";
                  currentEnumNum = 1;
                } else {
                  currentEnumName = "OFF";
                  currentEnumNum = 0;
                }
              }
              return true;
            }

            void Mixer::getPeak(int ctrlNum, int &L, int &R) {
              VAL(ctrlNum);
              L = (val.value & 0xff);
              R = ((val.value >> 8) & 0xff);
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
            //TODO: [Performance] Create a map for each structures and get values from there.
            bool Mixer::setDevVol(int ctrlNum, int L, int R, int M) {
              vr.dev = this -> default_mixer_dev;
              vr.ctrl = ctrlNum;
              vr.timestamp = ext.timestamp;

              VAL(ctrlNum);
              getExtensionInfo(ctrlNum);

              // Установка левого канала, правый не трогаем
              if (R == -1 && L && M == -1) {
                if ((ext.type == MIXT_STEREOSLIDER) || (ext.type
                    == MIXT_MONOSLIDER) || (ext.type == MIXT_MONOVU)
                    || (ext.type == MIXT_VALUE) || (ext.type == MIXT_SLIDER)
                    || (ext.type == MIXT_HEXVALUE) || (ext.type
                    == MIXT_HEXVALUE)) {
                  vr.value = ((L & 0x00ff) | ((((val.value >> 8) & 0xff)
                      & 0x00ff) << 8));
                } else if ((ext.type == MIXT_STEREOSLIDER16) || (ext.type
                    == MIXT_MONOSLIDER16)) {
                  vr.value = ((L * 10 & 0x00ff)
                      | ((((val.value >> 16) & 0xffff) & 0x00ff) << 16));
                }

                // Установка правого канала, левый не трогаем
              } else if (L == -1 && R && M == -1) {
                if ((ext.type == MIXT_STEREOSLIDER) || (ext.type
                    == MIXT_MONOSLIDER) || (ext.type == MIXT_MONOVU)
                    || (ext.type == MIXT_VALUE) || (ext.type == MIXT_SLIDER)
                    || (ext.type == MIXT_HEXVALUE) || (ext.type
                    == MIXT_HEXVALUE)) {
                  vr.value
                      = (((val.value & 0xff) & 0xff) | ((R & 0x00ff) << 8));
                } else if (ext.type == MIXT_STEREOSLIDER16) {
                  // vr.value = ( ( L*10 & 0x00ff ) | ( ( ( ( val.value >> 16) & 0xffff ) & 0x00ff ) << 16 ) );
                  vr.value = (((val.value & 0xff) & 0xff) | ((R * 10 & 0x00ff)
                      << 16));
                }

                // Установка левого и правого каналов
              } else if (L && R && M == -1) {
                if ((ext.type == MIXT_STEREOSLIDER) || (ext.type
                    == MIXT_MONOSLIDER) || (ext.type == MIXT_MONOVU)
                    || (ext.type == MIXT_VALUE) || (ext.type == MIXT_SLIDER)
                    || (ext.type == MIXT_HEXVALUE) || (ext.type
                    == MIXT_HEXVALUE)) {
                  vr.value = ((L & 0x00ff) | (R & 0x00ff) << 8);
                } else if (ext.type == MIXT_STEREOSLIDER16) {
                  vr.value = (((val.value & 0xff) & 0xff) | ((R * 10 & 0x00ff)
                      << 16));
                }
                // Установка моно канала
              } else if (M && L == -1 && R == -1) {
                if ((ext.type == MIXT_MONOSLIDER) || (ext.type == MIXT_MONOVU)) {
                  vr.value = ((M & 0xff) & 0xff);
                } else if (ext.type == MIXT_MONOSLIDER16) {
                  vr.value = (M * 10 & 0x00ff);
                }
              }

              if (ioctl(mixer_fd, SNDCTL_MIX_WRITE, &vr) == -1) {
                //                if (SILENTMEDIA_DEBUG == true) {
                //                  return (_debug("SNDCTL_MIX_WRITE"));
                //                }
              }
              return true;
            }

            bool Mixer::setDevVol(string ctrlName, int L, int R, int M) {
              int ctrlNum = -1;
              ctrlNum = getCtrlNumberByName(ctrlName);
              setDevVol(ctrlNum, L, R, M);
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
            bool Mixer::onOffDev(int ctrlNum, bool ON, bool L, bool R, bool M) {
              // Параметры для взятия значения текущего значения громкости контролера
              VAL(ctrlNum);

              // Параметры для записи значения громкости контролера
              vr.dev = this -> default_mixer_dev;
              vr.ctrl = ctrlNum;
              vr.timestamp = ext.timestamp;

              /*
               Если выключаем контролер, предварительно записываем текущие значения
               громкости, и ставим громкость в 0
               */
              if (!ON) {
                // Если выключам левый канал
                if (L & !R) {
                  devNumValL[ctrlNum] = (val.value & 0xff);
                  vr.value = (0 | ((((val.value >> 8) & 0xff) & 0x00ff) << 8));
                }
                // Если выключаем правый канал
                else if (R & !L) {
                  devNumValR[ctrlNum] = ((val.value >> 8) & 0xff);
                  vr.value = (((val.value & 0xff) & 0xff) | 0);
                  // Если выключаем оба канала
                } else if (L & R) {
                  devNumValL[ctrlNum] = (val.value & 0xff);
                  devNumValR[ctrlNum] = ((val.value >> 8) & 0xff);
                  vr.value = (0 | 0);
                } else if (M) {
                  devNumValM[ctrlNum] = (val.value & 0xff);
                  vr.value = (0);
                }
                // Если ON = 1 то восстанавливаем значение громкостей каналов
              } else if (ON) {
                // Если включам левый канал
                if (L & !R) {
                  vr.value = (((devNumValL[ctrlNum]) & 0x00ff) | ((((val.value
                      >> 8) & 0xff) & 0x00ff) << 8));
                } else if (R & !L) {
                  vr.value = (((val.value & 0xff) & 0xff)
                      | (((devNumValR[ctrlNum]) & 0x00ff) << 8));
                } else if (R & L) {
                  vr.value = (((devNumValL[ctrlNum]) & 0x00ff)
                      | ((devNumValR[ctrlNum]) & 0x00ff) << 8);
                } else if (M) {
                  vr.value = (devNumValM[ctrlNum] & 0x00ff);
                }
              }

              if (ioctl(mixer_fd, SNDCTL_MIX_WRITE, &vr) == -1) {
                //                if (SILENTMEDIA_DEBUG == true) {
                //                  return (_debug("SNDCTL_MIX_WRITE"));
                //                }
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
             @see Mixer::getCtrlInfo() \n
             список map < int, string > enumListVariant
             */
            bool Mixer::changeDevState(int ctrlNum, int state) {
              vr.value = state;
              VR(ctrlNum);
              return true;
            }

            bool Mixer::changeDevState(string ctrlName, int state) {
              int ctrlNum = -1;
              ctrlNum = getCtrlNumberByName(ctrlName);
              changeDevState(ctrlNum, state);
              return true;
            }

            bool Mixer::changeDevState(int ctrlNum,
                map<int, string> enumListVariant, string state) {
              for (map<int, string>::iterator it = enumListVariant.begin(); it
                  != enumListVariant.end(); ++it) {
                if (it -> second == state) {
                  vr.value = it -> first;
                  VR(ctrlNum);
                  return true;
                }
              }
              return false;
            }

            bool Mixer::changeDevState(string ctrlName,
                map<int, string> enumListVariant, string state) {
              int ctrlNum = -1;
              ctrlNum = getCtrlNumberByName(ctrlName);
              changeDevState(ctrlNum, enumListVariant, state);
              return true;
            }

            // ---------------------------------------------------------------------------
            //  Private methods
            // ---------------------------------------------------------------------------

            /**
             * Get mixer information.
             * @return true in success.
             */
            bool Mixer::getMixerInfo() {
              mi.dev = this -> default_mixer_dev;
              if (ioctl(mixer_fd, SNDCTL_MIXERINFO, &mi) == -1) {
                cerr << "Error in getMixerInfo (SNDCTL_MIXERINFO)" << endl;
                return false;
              }
              return true;
            }

            /**
             * Get system information.
             * @return true in success.
             */
            bool Mixer::getSysInfo() {
              if (ioctl(mixer_fd, SNDCTL_SYSINFO, &sysinfo) == -1) {
                cerr << "Error in SNDCTL_SYSINFO" << endl;
                return false;
              }
              return true;
            }

            /**
             * Find default mixer in system. Need, if more that one sound card.
             * present in system.
             * @return Number of mixers by default.
             */
            int Mixer::findDefaultMixerDev() {
              int best_pri = -2;
              for (int i = 0; i < sysinfo.nummixers; i++) {
                mi.dev = i;
                if (mi.priority < -1)
                  continue;
                if (mi.priority > best_pri) {
                  this -> default_mixer_dev = i;
                  best_pri = mi.priority;
                }
              }
              return this -> default_mixer_dev;
            }

            /**
             * Get number of mixer extension descriptor records.
             * @return true in success.
             */
            bool Mixer::getNumberOfControls() {
              numberOfControls = this -> default_mixer_dev;
              if (ioctl(mixer_fd, SNDCTL_MIX_NREXT, &numberOfControls) == -1) {
                cerr << "Error in getNumberOfControls (SNDCTL_MIX_NREXT)"
                    << endl;
                return false;
              }
              return true;
            }

            /**
             * Get a mixer extension descriptor.
             * @return true in success.
             */
            bool Mixer::getExtensionInfo(const int ctrlNum) {
              ext.dev = this -> default_mixer_dev;
              ext.ctrl = ctrlNum;
              if (ioctl(mixer_fd, SNDCTL_MIX_EXTINFO, &ext) == -1) {
                cerr << "Error in getExtensionInfo" << endl;
                return false;
              }
              return true;
            }

            /**
             * Prepare a map of available controls.
             * controller number => controller name.
             * @see void Mixer::getListOfCtrl.
             * @return true in success.
             */
            bool Mixer::getControlList() {
              for (int i = 1; i < numberOfControls; ++i) {
                if (this -> getExtensionInfo(i) == false) {
                  return false;
                } else {
                  const string ctrlName = ext.extname;
                  if (ext.type != MIXT_DEVROOT && ext.type != MIXT_GROUP
                      && ext.type != MIXT_MARKER) {
                    this -> fullCtrlList[i] = ctrlName;
                    //TODO: move "rec"
                    // add to map only "real" controllers
                    if (ctrlName.find("rec") == string::npos) {
                      this -> ctrlList[i] = ctrlName;
                    } else {
                      this -> recCtrlList.push_back(i);
                    }
                  }
                }
              }
              return true;
            }

            bool Mixer::VAL(int ctrlNum) {
              val.dev = this -> default_mixer_dev;
              val.ctrl = ctrlNum;
              val.timestamp = ext.timestamp;
              if (ioctl(mixer_fd, SNDCTL_MIX_READ, &val) == -1) {
                cerr << "Error in VAL, controller device No. " << ctrlNum
                    << endl;
                return false;
              }
              return true;
            }

            bool Mixer::VR(int ctrlNum) {
              vr.dev = this -> default_mixer_dev;
              vr.ctrl = ctrlNum;
              vr.timestamp = ext.timestamp;
              if (ioctl(mixer_fd, SNDCTL_MIX_WRITE, &vr) == -1) {
                cerr << "Error in VR" << endl;
                return false;
              }
              return true;
            }

            bool Mixer::EI(int ctrlNum) {
              ei.dev = this -> default_mixer_dev;
              ei.ctrl = ctrlNum;
              if (ioctl(mixer_fd, SNDCTL_MIX_ENUMINFO, &ei) == -1) {
                cerr << "Error in EI" << endl;
                return false;
              }
              return true;
            }

            string Mixer::getRecCtrlName(const string ctrlName) {
              return (ctrlName + REC_NAME);
            }

          }
        }
      }
    }
  }
}
