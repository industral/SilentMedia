#include "AudioInfo.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      AudioInfo * AudioInfo::_audioInfo = NULL;

      AudioInfo::AudioInfo() {
        //    ssystem(NULL) {
        //    this -> ssystem = SoundSystemManager::Instance();

        //    this -> audio = Audio::Instance();
      }

      AudioInfo::~AudioInfo() {
        //    cout << "AudioInfo Destructor.. " << endl;
      }

      AudioInfo * AudioInfo::Instance() {
        if (_audioInfo == NULL) {
          _audioInfo = new AudioInfo();
        }
        return _audioInfo;
      }

      void AudioInfo::setAudioParams(const string& fileId, const string& fileName, long fileSize, double totalTime,
          int channels, int sampleRate, double bitRate, int bitsPerSample) {
        this -> fileNameMap[fileId] = fileName;
        this -> fileSizeMap[fileId] = fileSize;
        this -> totalTimeMap[fileId] = totalTime;
        this -> channelsMap[fileId] = channels;
        this -> sampleRateMap[fileId] = sampleRate;
        this -> bitRateMap[fileId] = bitRate;
        this -> bitsPerSampleMap[fileId] = bitsPerSample;
      }

      // get methods
      long AudioInfo::getFileSize(const string &fileId) {
        return (this -> fileSizeMap[fileId]);
      }
      double AudioInfo::getTotalTime(const string &fileId) {
        return (this -> totalTimeMap[fileId]);
      }
      int AudioInfo::getChannels(const string &fileId) {
        return (this -> channelsMap[fileId]);
      }
      long AudioInfo::getSampleRate(const string &fileId) {
        return (this -> sampleRateMap[fileId]);
      }
      long AudioInfo::getBitRate(const string &fileId) {
        return (this -> bitRateMap[fileId]);
      }
      int AudioInfo::getBitsPerSample(const string &fileId) {
        return (this -> bitsPerSampleMap[fileId]);
      }

      // Additional information
      void AudioInfo::setTotalSamples(const string &fileId, double totalSamples) {
        this -> totalSamples[fileId] = totalSamples;
      }

      double AudioInfo::getTotalSamples(const string &fileId) {
        return (this -> totalSamples[fileId]);
      }

      void AudioInfo::setCurrentSamples(const string &fileId, const double &samples) {
        this -> currentSamples[fileId] = samples;
      }

      double AudioInfo::getCurrentSamples(const string &fileId) {
        return this -> currentSamples[fileId];
      }

      void AudioInfo::setVorbisComment(const string &fileId, const map <string, string> &vorbisComments) {
        this -> vorbisComments[fileId] = vorbisComments;
      }

      map <string, string> AudioInfo::getVorbisComments(const string &fileId) {
        return this -> vorbisComments[fileId];
      }

      //  void AudioInfo::begin(string id) {
      //    this -> ssystem -> allocateQuery(id);
      //    this -> ssystem -> setAudioParams(this -> channels, this -> sampleRate);
      //  }

      //  void AudioInfo::end(string id) {
      //    this -> audio = Audio::Instance();
      //
      //    this -> ssystem -> deAllocateQuery(id);
      //    this -> audio -> destroyObj(id);
      //  }

      // int AudioInfo::getDSPDev ( void ) const {
      //    return ( this -> ssystem -> getDSPDev() );
      // }

      //  void AudioInfo::clean(void) {
      //    this -> picData . clear();
      //    this -> vorbisComm . clear();
      //  }

      // void Audio::setCover ( string coverData ) {
      // //    if ( this -> ext == "flac" ) {
      // //       objs [ "flac" ] -> setPicture ( coverData );
      // //       FlacDecode flacdec;
      // //       flacdec . setPicture ( coverData );
      // //    }
      // }

      string AudioInfo::getFileNameByFileId(const string& fileId) {
        return (this -> fileIdMap[fileId]);
      }

      void AudioInfo::setFileId(const string& fileId, const string& fileName) {
        this -> fileIdMap[fileId] = fileName;
      }

      void AudioInfo::removeFileId(const string &fileId) {
        this -> fileIdMap[fileId].erase();
      }

      ::PlayStatus AudioInfo::getPlayStatus(const string & fileId) {
        return this -> playStatus[fileId];
      }

      void AudioInfo::setPlayStatus(const string & fileId, const ::PlayStatus & playStatus) {
        this -> playStatus[fileId] = playStatus;
      }

    }
  }
}
