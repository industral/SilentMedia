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

#ifndef __SS_SILENTMEDIA_OSS_MIXER_HPP_
#define __SS_SILENTMEDIA_OSS_MIXER_HPP_

#include "/usr/lib/oss/include/sys/soundcard.h"
#include <SML/config.hpp>
#include <SML/Audio/SoundSystem/AbstractSoundSystem.hpp>

/// Main Class
/**
   Main Class
 */
namespace SilentMedia {
   namespace SoundSystem {
      namespace OSS {
         class Mixer : virtual public AbstractSoundSystem {
            public:
               Mixer ( void );
               Mixer ( std::string dev );
               ~Mixer ( void );
               
               static Mixer * Instance ( void );
               bool init ( std::string dev ) ;

               int getUpdateCounter ( void );

               void getParentLabelByNum ( int parentNum, std::string &parentLabel, unsigned int &parentCount );
               void getParentNumByCtrlNum ( int ctrlNum, unsigned int &parentNum, unsigned int &prevParentNum, unsigned int &nextParentNum );
               void returnListOfAvaibleControlDev ( std::map < int, std::string > &listOfAvaibleCtrlDev );
               bool comaSeparatedListOfControls ( std::string &controlList, char param );

               bool getNumByCtrlName ( std::string param, int &control_num );
               bool getNameByCtrlNum ( int param, std::string &control_name );

               bool checkRecAvail ( std::string param, int &control_num );
               bool checkRecAvail ( std::string param );
               bool checkRecAvail ( int controlNum );

               bool getDevInfo ( std::string                      param,
                                 std::string &                    ctrlLabel,
                                 int &                            ctrlNum,
                                 unsigned int &                   ctrlParent,
                                 int &                            numRecDev,
                                 bool &                           recModeAvail,
                                 bool &                           recModeStatus,
                                 short int &                      ctrlMode,
                                 short int &                      ctlStatus,
                                 int &                            L,
                                 int &                            R,
                                 int &                            M,
                                 int &                            minCtrlValue,
                                 int &                            maxCtrlValue,
                                 bool &                           skipDev,
                                 std::map < int, std::string > &  enumListVariant,
                                 std::string &                    currentEnumName,
                                 int &                            currentEnumNum,
                                 int &                            ctrlTypeName,
                                 int &                            ctrlFlag
                               );

               void getPeak ( int ctrlNum, int &L, int &R );

               bool setDevVol ( int control_num, int L, int R, int M );
               bool setDevVol ( std::string ctrlName, int L, int R, int M );
               bool onOffDev ( int control_num, bool ON, bool L, bool R, bool M );

               bool changeDevState ( int control_num, int state );
               bool changeDevState ( std::string ctrlName, int state );
               bool changeDevState ( int ctrlNum, std::map < int, std::string > enumListVariant, std::string state );
               bool changeDevState ( std::string ctrlName, std::map < int, std::string > enumListVariant, std::string state );
            private:
               static Mixer * _mixer;
         // private methods
               int findDefaultMixerDev ( void );
               bool _OSS_NREXT ( void );
               bool _OSS_SNDCTL_SYSINFO ( void );
               bool getControlList ( void );
               bool VAL ( int ctrlNum );
               bool VR ( int ctrlNum );
               bool MI ( void );
               bool EI ( int ctrlNum );
               bool EXT ( int ctrlNum );

               oss_sysinfo sysinfo; // SNDCTL_SYSINFO
               oss_mixerinfo mi;
               oss_mixext ext; // SNDCTL_MIX_EXTINFO
               oss_mixer_value vr, val; // SNDCTL_MIX_WRITE SNDCTL_MIX_READ
               oss_mixer_enuminfo ei; // SNDCTL_MIX_ENUMINFO
               oss_audioinfo ai; // SNDCTL_AUDIOINFO

               int default_mixer_dev;
               int mixer_fd;
               int nrext;

               std::map < int, int > devNumValR;
               std::map < int, int > devNumValL;
               std::map < int, int > devNumValM;

               std::string ListOfDevice;
               std::map < int, std::string > listOfReadableCtrlDev;
               std::map < int, std::string > listOfWriteableCtrlDev;
               std::map < int, std::string > listOfAvaibleCtrlDev;
               std::map < int, std::string > fullCtrlList;
               std::map < int, int > parentList;
               std::map < int, std::string > listOfSkipCtrl;
         };
      }
   }
}

#endif
