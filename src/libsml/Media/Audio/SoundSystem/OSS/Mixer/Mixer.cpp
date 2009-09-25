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

            int Mixer::getCtrlNumberByName(const string ctrlName) {
              int ctrlNum = -1;
              for (map<int, string>::iterator it = this -> fullCtrlList.begin(); it
                  != this -> fullCtrlList.end(); ++it) {
                if (it -> second == ctrlName) {
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

            ctrlInfo Mixer::getCtrlInfo(const int ctrlNum) {
              ctrlInfo ctrlInfoObj;

              // get controller name
              const string ctrlName = this -> ctrlList[ctrlNum];

              // get controller name, without it class (up to dot)
              ctrlInfoObj.ctrlLabel = ctrlName.substr(ctrlName.find(".") + 1,
                  ctrlName.size());

              /* Check if controller has record capabilities */

              ctrlInfoObj.numRecDev = -1;
              ctrlInfoObj.recModeAvail = false;
              ctrlInfoObj.recModeStatus = false;

              if (this -> checkRecAvail(ctrlInfoObj.ctrlLabel)) {
                // get record controller name
                const string recCtrlName = this -> getRecCtrlName(ctrlName);

                // get record controller number
                const int recCtrlNumber = this -> getCtrlNumberByName(
                    recCtrlName);

                for (list<int>::iterator it = this -> recCtrlList.begin(); it
                    != this -> recCtrlList.end(); ++it) {

                  const int foundRecCtrlNumber = *it;
                  if (foundRecCtrlNumber == recCtrlNumber) {
                    ctrlInfoObj.numRecDev = foundRecCtrlNumber;
                    ctrlInfoObj.recModeAvail = true;

                    VAL(foundRecCtrlNumber);
                    (val.value & 0xff) ? ctrlInfoObj.recModeStatus = true
                        : ctrlInfoObj.recModeStatus = false;
                  }
                }
              }

              // get controller capabilities
              this -> VAL(ctrlNum);
              this -> getExtensionInfo(ctrlNum);

              // set on/off based on value
              val.value ? ctrlInfoObj.on = true : ctrlInfoObj.on = false;

              ctrlInfoObj.minCtrlValue = ext.minvalue;
              ctrlInfoObj.maxCtrlValue = ext.maxvalue;
              ctrlInfoObj.ctrlFlag = ext.flags;
              ctrlInfoObj.ctrlTypeName = ext.type;

              ctrlInfoObj.stereo = false;
              ctrlInfoObj.L = -1;
              ctrlInfoObj.R = -1;
              ctrlInfoObj.M = -1;

              // MIXT_STEREOSLIDER: Stereo volume slider with 8 bit precision
              if (ext.type == MIXT_STEREOSLIDER) {
                ctrlInfoObj.stereo = true;
                ctrlInfoObj.L = (val.value & 0xff);
                ctrlInfoObj.R = ((val.value >> 8) & 0xff);
                // MIXT_STEREOSLIDER16: Stereo volume slider with 16 bit precision
              } else if (ext.type == MIXT_STEREOSLIDER16) {
                ctrlInfoObj.stereo = true;
                ctrlInfoObj.L = (val.value & 0xffff);
                ctrlInfoObj.R = ((val.value >> 16) & 0xffff);
                // dB
                if (ext.flags & MIXF_CENTIBEL) {
                  ctrlInfoObj.minCtrlValue = ext.minvalue / 10;
                  ctrlInfoObj.maxCtrlValue = ext.maxvalue / 10;
                  ctrlInfoObj.L = ctrlInfoObj.L / 10 + ctrlInfoObj.L % 10;
                  ctrlInfoObj.R = ctrlInfoObj.R / 10 + ctrlInfoObj.R % 10;
                }
                // MIXT_MONOSLIDER16: Mono volume slider with 16 bit value field
              } else if (ext.type == MIXT_MONOSLIDER16) {
                ctrlInfoObj.M = (val.value & 0xffff);
                // dB
                if (ext.flags & MIXF_CENTIBEL) {
                  ctrlInfoObj.minCtrlValue = ext.minvalue / 10;
                  ctrlInfoObj.maxCtrlValue = ext.maxvalue / 10;
                  ctrlInfoObj.M = ctrlInfoObj.M / 10;
                }
                // show input/output level
                // MIXT_STEREOVU:   Mixer control type
                // MIXT_STEREOPEAK: Stereo peak meter control
              } else if ((ext.type == MIXT_STEREOVU) || (ext.type
                  == MIXT_STEREOPEAK)) {
                ctrlInfoObj.L = (val.value & 0xff);
                ctrlInfoObj.R = ((val.value >> 8) & 0xff);
                // MIXT_ENUM: Enumerated mixer control
              } else if (ext.type == MIXT_ENUM) {
                EI(ctrlNum);

                for (int i = 0; i < ei.nvalues; ++i) {
                  if (ext.enum_present[i / 8] & (1 << (i % 8))) {
                    ctrlInfoObj.enumListVariant[i] = (ei.strings
                        + ei.strindex[i]);
                  }
                }

                ctrlInfoObj.currentEnumNum = val.value & 0xff; // Номер текущего елемента
                ctrlInfoObj.currentEnumName
                    = ctrlInfoObj.enumListVariant[ctrlInfoObj.currentEnumNum]; //  Имя текущего элемента
                // MIXT_MONOSLIDER: 8 bit mono volume slider
                // MIXT_MONOVU:     Undefined control
              } else if ((ext.type == MIXT_MONOSLIDER) || (ext.type
                  == MIXT_MONOVU)) {
                ctrlInfoObj.M = (val.value & 0xff);
                // MIXT_SLIDER:   Mono volume slider with expanded range
                // MIXT_VALUE:    General purpose decimal value mixer control
                // MIXT_HEXVALUE: Hexadecimal mixer value control
              } else if ((ext.type == MIXT_SLIDER) || (ext.type == MIXT_VALUE)
                  || (ext.type == MIXT_HEXVALUE)) {
                ctrlInfoObj.M = val.value;
                // MIXT_ONOFF: Hexadecimal mixer value control
              } else if (ext.type == MIXT_ONOFF) {
                if (val.value) {
                  ctrlInfoObj.currentEnumName = "ON";
                  ctrlInfoObj.currentEnumNum = 1;
                } else {
                  ctrlInfoObj.currentEnumName = "OFF";
                  ctrlInfoObj.currentEnumNum = 0;
                }
              }
              return ctrlInfoObj;
            }

            void Mixer::getPeak(int ctrlNum, int &L, int &R) {
              VAL(ctrlNum);
              L = (val.value & 0xff);
              R = ((val.value >> 8) & 0xff);
            }

            bool Mixer::setDevVol(const int ctrlNum, const int L, const int R,
                const int M) {
              int value = this -> valueInfoMap[ctrlNum].value;
              int type = this -> extensionInfoMap[ctrlNum].type;

              vr.dev = this -> default_mixer_dev;
              vr.ctrl = ctrlNum;
              vr.timestamp = this -> extensionInfoMap[ctrlNum].timestamp;

              // set left channel, don't touch right channel
              if (R == -1 && L && M == -1) {
                if ((type == MIXT_STEREOSLIDER) || (type == MIXT_MONOSLIDER)
                    || (type == MIXT_MONOVU) || (type == MIXT_VALUE) || (type
                    == MIXT_SLIDER) || (type == MIXT_HEXVALUE) || (type
                    == MIXT_HEXVALUE)) {
                  vr.value = ((L & 0x00ff) | ((((value >> 8) & 0xff) & 0x00ff)
                      << 8));
                } else if ((type == MIXT_STEREOSLIDER16) || (type
                    == MIXT_MONOSLIDER16)) {
                  vr.value = ((L * 10 & 0x00ff) | ((((value >> 16) & 0xffff)
                      & 0x00ff) << 16));
                }

                // set right channel, don't touch left channel
              } else if (L == -1 && R && M == -1) {
                if ((type == MIXT_STEREOSLIDER) || (type == MIXT_MONOSLIDER)
                    || (type == MIXT_MONOVU) || (type == MIXT_VALUE) || (type
                    == MIXT_SLIDER) || (type == MIXT_HEXVALUE) || (type
                    == MIXT_HEXVALUE)) {
                  vr.value = (((value & 0xff) & 0xff) | ((R & 0x00ff) << 8));
                } else if (type == MIXT_STEREOSLIDER16) {
                  vr.value = (((value & 0xff) & 0xff) | ((R * 10 & 0x00ff)
                      << 16));
                }

                // set right & left channel both
              } else if (L && R && M == -1) {
                if ((type == MIXT_STEREOSLIDER) || (type == MIXT_MONOSLIDER)
                    || (type == MIXT_MONOVU) || (type == MIXT_VALUE) || (type
                    == MIXT_SLIDER) || (type == MIXT_HEXVALUE) || (type
                    == MIXT_HEXVALUE)) {
                  vr.value = ((L & 0x00ff) | (R & 0x00ff) << 8);
                } else if (type == MIXT_STEREOSLIDER16) {
                  vr.value = (((value & 0xff) & 0xff) | ((R * 10 & 0x00ff)
                      << 16));
                }
                // set mono channel
              } else if (M && L == -1 && R == -1) {
                if ((type == MIXT_MONOSLIDER) || (type == MIXT_MONOVU)) {
                  vr.value = ((M & 0xff) & 0xff);
                } else if (type == MIXT_MONOSLIDER16) {
                  vr.value = (M * 10 & 0x00ff);
                }
              }

              // don't forget to update map!
              this -> valueInfoMap[ctrlNum].value = vr.value;

              if (ioctl(mixer_fd, SNDCTL_MIX_WRITE, &vr) == -1) {
                cerr << "Error in SNDCTL_MIX_WRITE" << endl;
                return false;
              }
              return true;
            }

            bool Mixer::toggleCtrl(const int ctrlNum, const bool status,
                const bool L, const bool R, const bool M) {
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
              if (!status) {
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
              } else if (status) {
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

            bool Mixer::changeDevState(const int ctrlNum, const int state) {
              vr.value = state;
              if (VR(ctrlNum) == false) {
                return false;
              }
              return true;
            }

            // ---------------------------------------------------------------------------
            //  Private methods
            // ---------------------------------------------------------------------------

            bool Mixer::getMixerInfo() {
              mi.dev = this -> default_mixer_dev;
              if (ioctl(mixer_fd, SNDCTL_MIXERINFO, &mi) == -1) {
                cerr << "Error in getMixerInfo (SNDCTL_MIXERINFO)" << endl;
                return false;
              }
              return true;
            }

            bool Mixer::getSysInfo() {
              if (ioctl(mixer_fd, SNDCTL_SYSINFO, &sysinfo) == -1) {
                cerr << "Error in SNDCTL_SYSINFO" << endl;
                return false;
              }
              return true;
            }

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

            bool Mixer::getNumberOfControls() {
              numberOfControls = this -> default_mixer_dev;
              if (ioctl(mixer_fd, SNDCTL_MIX_NREXT, &numberOfControls) == -1) {
                cerr << "Error in getNumberOfControls (SNDCTL_MIX_NREXT)"
                    << endl;
                return false;
              }
              return true;
            }

            bool Mixer::getExtensionInfo(const int ctrlNum) {
              ext.dev = this -> default_mixer_dev;
              ext.ctrl = ctrlNum;
              if (ioctl(mixer_fd, SNDCTL_MIX_EXTINFO, &ext) == -1) {
                cerr << "Error in getExtensionInfo" << endl;
                return false;
              }
              return true;
            }

            bool Mixer::getControlList() {
              for (int i = 1; i < numberOfControls; ++i) {
                if (this -> getExtensionInfo(i) == false) {
                  return false;
                } else {
                  // create map for ext info
                  this -> extensionInfoMap[i] = this -> ext;

                  const string ctrlName = ext.extname;
                  if (ext.type != MIXT_DEVROOT && ext.type != MIXT_GROUP
                      && ext.type != MIXT_MARKER) {
                    this -> fullCtrlList[i] = ctrlName;

                    // create map for values
                    if (this -> VAL(i) == false) {
                      return false;
                    } else {
                      this -> valueInfoMap[i] = this -> val;
                    }

                    // add to map only "real" controllers
                    if (ctrlName.find(REC_NAME) == string::npos) {
                      this -> ctrlList[i] = ctrlName;
                    } else {
                      this -> recCtrlList.push_back(i);
                    }
                  }
                }
              }
              return true;
            }

            bool Mixer::VAL(const int ctrlNum) {
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
              vr.timestamp = this -> extensionInfoMap[ctrlNum].timestamp;
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
