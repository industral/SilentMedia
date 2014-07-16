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

            /**
             * Appendix value for record capability devices.
             */
            const string REC_NAME = ".rec";

            /**
             *  Controller information
             *  @see Mixer::getCtrlInfo(const int ctrlNum)
             *
             *  @param ctrlLabel controller name
             *  @param numRecDev number of record controller
             *  @param recModeAvail controller record capability
             *  @param recModeStatus controller record status
             *  @param stereo controller channel state
             *  @param on controller status
             *  @param L value of left channel
             *  @param R value of right channel
             *  @param M value of mono channel
             *  @param minCtrlValue min controller value
             *  @param maxCtrlValue max controller value
             *  @param enumListVariant for special type of controllers, contains
             *  id => value, where id just iteration number, and value - name of
             *  option
             *  @param currentEnumName contains current enum name for
             *  enumListVariant
             *  @param currentEnumNum contains current enum number for
             *  enumListVariant
             *  @param ctrlTypeName controller type name
             *  @param ctrlFlag controller flag
             */
            struct ctrlInfo {
                string ctrlLabel;
                int numRecDev;
                bool recModeAvail;
                bool recModeStatus;
                bool stereo;
                bool on;
                int L;
                int R;
                int M;
                int minCtrlValue;
                int maxCtrlValue;
                map<int, string> enumListVariant;
                string currentEnumName;
                int currentEnumNum;
                int ctrlTypeName;
                int ctrlFlag;
            };

            class Mixer {
              public:
                /**
                 * Default constructor.
                 */
                Mixer();

                /**
                 * Overload constructor.
                 * @param[in] dev mixer name ( e.g. "/dev/mixer" ).
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
                 * @param[in] dev mixer name ( e.g. "/dev/mixer" ).
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
                 * @param[in] ctrlName controller name.
                 *
                 * @return controller number in system, -1 in error.
                 *
                 * @code
                 *   // find controller number by it name
                 *   string controlName = "vol"; // define searching name
                 *   int ctrlNumber = ossmix -> getCtrlNumberByName (controlName);
                 * @endcode
                 */
                int getCtrlNumberByName(const string ctrlName);

                /**
                 * Check controller to be able for recording.
                 * @param[in] ctrlName controller name.
                 * @return true if record capabilities available.
                 */
                bool checkRecAvail(const string ctrlName);

                /**
                 * Get various information about controller.
                 * @param[in] ctrlNum controller number.
                 * @return ctrlInfo structure.
                 */
                ctrlInfo getCtrlInfo(const int ctrlNum);

                void getPeak(int ctrlNum, int &L, int &R);

                /**
                 * Set volume.
                 *
                 * @param[in] ctrlNum controller number.
                 * @param[in] L left channel value.
                 * @param[in] R right channel value.
                 * @param[in] M mono channel value.
                 *
                 * Set to -1 controller, which channel you whan't to change.
                 * For mono: L = -1, R = -1, M = some value
                 * For stereo: L = some value, R = some value, M = -1;
                 * @code
                 *   const int ctrlNum = 2; // controller number
                 *   ssmix -> setDevVol(ctrlNum, -1, 80);
                 * @endcode
                 * The above code, set right channel to value 80, and not change
                 * left channel.
                 *
                 * @return true in success.
                 */
                bool setDevVol(const int ctrlNum, const int L, const int R,
                    const int M);

                /**
                 * Toggle controller (ON/OFF).
                 * @param[in] ctrlNum controller number.
                 * @param[in] status changing status, true - ON, false - OFF.
                 * @param[in] L, R, M - action to appropriate channel.
                 * @return true in success.
                 *
                 * @note
                 *   Please note, that on turn on channels, restore there previous
                 *   values.
                 *
                 * @code
                 *   const int ctrlNum = 2;
                 *   // turn off left channel
                 *   ssmix -> toggleCtrl (ctrlNum, 0, 1, 0);
                 *   // turn off right channel
                 *   ssmix -> toggleCtrl (ctrlNum, 0, 0, 1);
                 *   // turn off both channels
                 *   ssmix -> toggleCtrl (ctrlNum, 0, 1, 1);
                 *   // turn on both channels
                 *   ssmix -> toggleCtrl (ctrlNum, 1, 1, 1);
                 * @endcode
                 */
                bool toggleCtrl(const int ctrlNum, const bool status,
                    const bool L, const bool R, const bool M);

                /**
                 * Toggle/change controller status.
                 * @param[in] ctrlNum controller number.
                 * @param[in] state changing status (0/1 - for ON/OFF,
                 * 0 - ... - for ENUM )
                 * @return true in success.
                 *
                 * @code
                 *   const int ctrlNum = 38; // controller number
                 *
                 *   // changing status for enum
                 *   ssmix -> changeDevState (ctrlNum, 5);
                 * @endcode
                 */
                bool changeDevState(const int ctrlNum, const int state);
              private:

                /**
                 * Get mixer information.
                 * @return true in success.
                 */
                bool getMixerInfo();

                /**
                 * Get system information.
                 * @return true in success.
                 */
                bool getSysInfo();

                /**
                 * Find default mixer in system. Need, if more that one sound card.
                 * present in system.
                 * @return Number of mixers by default.
                 */
                int findDefaultMixerDev();

                /**
                 * Get number of mixer extension descriptor records.
                 * @return true in success.
                 */
                bool getNumberOfControls();

                /**
                 * Get a mixer extension descriptor.
                 * @return true in success.
                 */
                bool getExtensionInfo(const int ctrlNum);

                /**
                 * Prepare a map of available controls.
                 * controller number => controller name.
                 * @return true in success.
                 */
                bool getControlList();
                bool VAL(const int ctrlNum);
                bool VR(int ctrlNum);

                bool EI(int ctrlNum);

                /**
                 * Return a name of record controller, e.g. "vol" => "vol.rec".
                 * @param[in] ctrlName controller name.
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

                map<int, oss_mixext> extensionInfoMap;
                map<int, oss_mixer_value> valueInfoMap;
            };
          }
        }
      }
    }
  }
}

#endif
