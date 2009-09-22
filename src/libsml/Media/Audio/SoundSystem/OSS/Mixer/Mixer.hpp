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

#ifndef _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_OSS_MIXER_MIXER_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_SOUNDSYSTEM_OSS_MIXER_MIXER_HPP_

// include main
#include <libsml/include.hpp>

#include "/usr/lib/oss/include/sys/soundcard.h"

// C
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

using namespace std;

/// Main Class
/**
 Main Class
 */
namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace OSS {
          namespace Mixer {
            class Mixer {
              public:
                Mixer();
                Mixer(const string dev);
                ~Mixer();

                bool init(const string dev);
                int getUpdateCounter();

                void getParentLabelByNum(int parentNum, string &parentLabel,
                    unsigned int &parentCount);
                void getParentNumByCtrlNum(int ctrlNum,
                    unsigned int &parentNum, unsigned int &prevParentNum,
                    unsigned int &nextParentNum);
                map<int, string> getListOfCtrl() const;

                bool getNumByCtrlName(string param, int &control_num);
                bool getNameByCtrlNum(int param, string &control_name);

                bool checkRecAvail(string param, int &control_num);
                bool checkRecAvail(string param);
                bool checkRecAvail(int controlNum);

                bool getDevInfo(string param, string & ctrlLabel,
                    int & ctrlNum, unsigned int & ctrlParent, int & numRecDev,
                    bool & recModeAvail, bool & recModeStatus,
                    short int & ctrlMode, short int & ctlStatus, int & L,
                    int & R, int & M, int & minCtrlValue, int & maxCtrlValue,
                    bool & skipDev, map<int, string> & enumListVariant,
                    string & currentEnumName, int & currentEnumNum,
                    int & ctrlTypeName, int & ctrlFlag);

                void getPeak(int ctrlNum, int &L, int &R);

                bool setDevVol(int control_num, int L, int R, int M);
                bool setDevVol(string ctrlName, int L, int R, int M);
                bool onOffDev(int control_num, bool ON, bool L, bool R, bool M);

                bool changeDevState(int control_num, int state);
                bool changeDevState(string ctrlName, int state);
                bool changeDevState(int ctrlNum,
                    map<int, string> enumListVariant, string state);
                bool changeDevState(string ctrlName,
                    map<int, string> enumListVariant, string state);
              private:
                bool getMixerInfo();
                bool getSysInfo();
                int findDefaultMixerDev();
                bool getNumberOfControls();
                bool getExtensionInfo(const int ctrlNum);
                bool getControlList();
                bool VAL(int ctrlNum);
                bool VR(int ctrlNum);

                bool EI(int ctrlNum);

                oss_sysinfo sysinfo; // SNDCTL_SYSINFO
                oss_mixerinfo mi;
                oss_mixext ext; // SNDCTL_MIX_EXTINFO
                oss_mixer_value vr, val; // SNDCTL_MIX_WRITE SNDCTL_MIX_READ
                oss_mixer_enuminfo ei; // SNDCTL_MIX_ENUMINFO
                oss_audioinfo ai; // SNDCTL_AUDIOINFO

                int default_mixer_dev;
                int mixer_fd;
                // number of controls in system
                int numberOfControls;

                map<int, int> devNumValR;
                map<int, int> devNumValL;
                map<int, int> devNumValM;

                string ListOfDevice;
                map<int, string> listOfReadableCtrlDev;
                map<int, string> listOfWriteableCtrlDev;
                map<int, string> listOfAvaibleCtrlDev;
                map<int, string> fullCtrlList;
                map<int, int> parentList;
                map<int, string> listOfSkipCtrl;
            };
          }
        }
      }
    }
  }
}

#endif
