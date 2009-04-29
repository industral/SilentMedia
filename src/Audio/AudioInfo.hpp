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

#ifndef _SILENTMEDIA_AUDIOINFO_HPP_
#define _SILENTMEDIA_AUDIOINFO_HPP_

#include <libsml/include.hpp>

namespace SilentMedia {
  namespace Audio {
    class AudioInfo {
      public:
        ~AudioInfo();

        static AudioInfo * Instance(void);

        //      void begin(string id);
        //      void end(string id);

        void setAudioParams(const string& fileId, const string& fileName,
            long fileSize = -1, double totalTime = -1, int channels = -1,
            int sampleRate = -1, double bitRate = -1, int bitsPerSample = -1);

        //      void clean(void);
        //          bool setAudioParams ( void );

        map < string, string > getVorbisComment(void) {
          return (this -> vorbisComm);
        }

        //      multimap < ::FLAC__StreamMetadata_Picture_Type, string > getPicture(
        //          void) {
        //        return (this -> picData);
        //      }

        //      void setPicture(multimap < ::FLAC__StreamMetadata_Picture_Type,
        //          string > picData) {
        //        this -> picData = picData;
        //      }

        void setVorbisComment(map < string, string > vorbisComm) {
          this -> vorbisComm = vorbisComm;
        }

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

        string getFileNameByFileId(const string& fileId);
        void setFileId(const string& fileId, const string& fileName);

      private:
        // self instance variable
        static AudioInfo * _audioInfo;

        // storage hashmap fileId => fileName
        map < string, string > fileIdMap;

        bool playCheck;

        //      map < string, map < string, void * > > archive;
        map < string, string > fileNameMap;
        map < string, long > fileSizeMap;
        map < string, double > totalTimeMap;
        map < string, int > channelsMap;
        map < string, int > sampleRateMap;
        map < string, double > bitRateMap;
        map < string, int > bitsPerSampleMap;

        // Additional information
        map < string, double > totalSamples;
        map < string, double > currentSamples;

        //      multimap < ::FLAC__StreamMetadata_Picture_Type, string > picData;
        map < string, string > vorbisComm; // Vorbis Comment

        map < string, string > vorbisTags;
        map < string, string > coverData;
      protected:
        AudioInfo();
    };
  }
}

#endif
