#ifndef _SILENTMEDIA_MEDIA_AUDIO_AUDIOINFO_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_AUDIOINFO_HPP_

#include <libsml/include.hpp>

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {

      class AudioInfo {
        public:
          ~AudioInfo();

          static AudioInfo * Instance();

          //      void begin(string id);
          //      void end(string id);

          void setAudioParams(const string& fileId, const string& fileName, long fileSize = -1, double totalTime = -1,
              int channels = -1, int sampleRate = -1, double bitRate = -1, int bitsPerSample = -1);

          //      void clean(void);
          //          bool setAudioParams ( void );

          //      multimap < ::FLAC__StreamMetadata_Picture_Type, string > getPicture(
          //          void) {
          //        return (this -> picData);
          //      }

          //      void setPicture(multimap < ::FLAC__StreamMetadata_Picture_Type,
          //          string > picData) {
          //        this -> picData = picData;
          //      }

          // get func
          long getFileSize(const string &fileId);
          double getTotalTime(const string &fileId);
          int getChannels(const string &fileId);
          long getSampleRate(const string &fileId);
          long getBitRate(const string &fileId);
          int getBitsPerSample(const string &fileId);

          // Additional information
          void setTotalSamples(const string &fileId, double totalSamples);
          double getTotalSamples(const string &fileId);

          void setCurrentSamples(const string &fileId, const double &samples);
          double getCurrentSamples(const string &fileId);

          void setVorbisComment(const string &fileId, const map <string, string> &vorbisComments);
          map <string, string> getVorbisComments(const string &fileId);

          string getFileNameByFileId(const string& fileId);
          void setFileId(const string& fileId, const string& fileName);
          void removeFileId(const string &fileId);

          ::PlayStatus getPlayStatus(const string & fileId);
          void setPlayStatus(const string & fileId, const PlayStatus & playStatus);

        private:
          // self instance variable
          static AudioInfo * _audioInfo;

          // storage hashmap fileId => fileName
          map <string, string> fileIdMap;

          //      map < string, map < string, void * > > archive;
          map <string, string> fileNameMap;
          map <string, long> fileSizeMap;
          map <string, double> totalTimeMap;
          map <string, int> channelsMap;
          map <string, int> sampleRateMap;
          map <string, double> bitRateMap;
          map <string, int> bitsPerSampleMap;

          // Additional information
          map <string, double> totalSamples;
          map <string, double> currentSamples;

          //      multimap < ::FLAC__StreamMetadata_Picture_Type, string > picData;

          // Vorbis Comment
          map <string, map <string, string> > vorbisComments;

          map <string, string> vorbisTags;
          map <string, string> coverData;
          map <string, ::PlayStatus> playStatus;

        protected:
          AudioInfo();
      };
    }
  }
}

#endif
