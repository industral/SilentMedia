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

#ifndef _SILENTMEDIA_DECODEDDATA_HPP_
#define _SILENTMEDIA_DECODEDDATA_HPP_

#include <iostream>
#include <string>
#include <map>

using namespace std;

namespace SilentMedia {
  namespace Audio {
    class AudioInfo {
      public:
        ~AudioInfo();

        static AudioInfo * Instance(void);

        //      void begin(string id);
        //      void end(string id);

        void setAudioParams(string fileName, long int fileSize = -1,
            double totalTime = -1, int channels = -1, int sampleRate = -1,
            double bitRate = -1, int bitsPerSample = -1);

        //      void clean(void);
        //          bool setParams ( void );

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
        unsigned long int getFileSize(void) {
          return (this -> fileSize);
        }
        double getTotalTime(void) {
          return (this -> totalTime);
        }
        unsigned int getChannels(void) {
          return (this -> channels);
        }
        unsigned int getSampleRate(void) {
          return (this -> sampleRate);
        }
        double getBitRate(void) {
          return (this -> bitRate);
        }
        short int getBitsPerSample(void) {
          return (this -> bitsPerSample);
        }

        bool play(const void * buf, string id);
        //          int getDSPDev ( void ) const;
      private:
        AudioInfo();

        static AudioInfo * _info;

        bool playCheck;

        //      map < string, map < string, void * > > archive;
        string fileName;
        long int fileSize;
        double totalTime;
        int channels;
        int sampleRate;
        double bitRate;
        int bitsPerSample;

        //      multimap < ::FLAC__StreamMetadata_Picture_Type, string > picData;
        map < string, string > vorbisComm; // Vorbis Comment

        map < string, string > vorbisTags;
        map < string, string > coverData;

    };
  }
}

#endif
