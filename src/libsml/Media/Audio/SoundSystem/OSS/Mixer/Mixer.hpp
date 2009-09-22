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

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace SoundSystem {
        namespace OSS {
          namespace Mixer {

            const string REC_NAME = ".rec";

            class Mixer {
              public:
                /**
                 * Default constructor.
                 */
                Mixer();

                /**
                 * Overload constructor.
                 * @param dev mixer name ( e.g. "/dev/mixer" ).
                 * @see Mixer()
                 */
                Mixer(const string dev);

                /**
                 * Default destructor.
                 */
                ~Mixer();

                /**
                 * Mixer initialization. Use this method to initialized mixer
                 * with default mixer.
                 * @see init(const string dev)
                 * @return true in success.
                 */
                bool init();

                /**
                 * Mixer initialization. Use this method to setup prefer mixer.
                 * @param dev mixer name ( e.g. "/dev/mixer" ).
                 * @return true in success.
                 * @code
                 *   Mixer * ossmix = new Mixer();
                 *   ossmix -> init("/dev/mixer");
                 * @endcode
                 */
                bool init(const string dev);

                /**
                 * Get current value of mixer change incrementing.
                 * @return current number.
                 */
                int getUpdateCounter();

                /**
                 * Get a list of available controllers.
                 * You should use this method to get a list of available controllers
                 * in system.
                 * @return list of available controllers in system.
                 */
                map<int, string> getListOfCtrl() const;

                /**
                 * Get controller number by it name.
                 * @return controller number in system.
                 * @code
                 *   // find controller number by it name
                 *   string controlName = "vol"; // define searching name
                 *   int ctrlNumber = ossmix -> getCtrlNumberByName (controlName);
                 * @endcode
                 */
                int getCtrlNumberByName(const string param);

                /**
                 * Check controller to be able for recording.
                 * @param ctrlName controller name.
                 * @return true if record capabilities available.
                 */
                bool checkRecAvail(const string ctrlName);

                bool getCtrlInfo(const int ctrlNum, string & ctrlLabel, int & numRecDev,
                    bool & recModeAvail, bool & recModeStatus,
                    bool & stereo, bool & on, int & L, int & R, int & M,
                    int & minCtrlValue, int & maxCtrlValue,
                    map<int, string> & enumListVariant,
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

                /**
                 * Return a name of record controller, e.g. "vol" => "vol.rec".
                 * @param ctrlName controller name.
                 * @return record controller name.
                 */
                string getRecCtrlName(const string ctrlName);

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

                map<int, string> ctrlList;
                map<int, string> fullCtrlList;

                // store a list (there control numbers) of record controls
                list<int> recCtrlList;
            };
          }
        }
      }
    }
  }
}

#endif
