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

#ifndef _SILENTMEDIA_MEDIA_AUDIO_AUDIOPROXY_HPP_
#define _SILENTMEDIA_MEDIA_AUDIO_AUDIOPROXY_HPP_

// main include
#include <libsml/include.hpp>

/*
 * We should include AudioInfo, SoundSystem
 */
#include "AudioInfo.hpp"
#include "SoundSystem/SoundSystem.hpp"

using namespace std;

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      class AudioProxy {
        public:
          AudioProxy();
          ~AudioProxy();

          // AudioInfo methods
          string getFileNameByFileId(const string& fileId);

          // SoundSystem methods
          /**
           * Start playback file.
           * @param[in] buf data audio buffer.
           * @param[in] bufSize buffer size.
           */
          int write(void *buf, const int &bufSize);

          /**
           * Set audio information. Set information about file name, file size,
           * total time, channels, sample rate, bits rate, bits per sample.
           * This method update information in AudioInfo.
           * This method should call every time when we get information about
           * track.
           * @params[in] fileName name of track that playback.
           * @param[in] fileSize size of file in bytes.
           * @param[in] totalTime total time of track.
           * @param[in] channels channels in audio (1/2).
           * @param[in] sampleRate sample rate of track (44100/48000).
           * @param[in] bitRate bit rate of track (224 kbps).
           * @param[in] bitsPerSample bits per sample in track(16/24/32).
           */
          void setAudioParams(const string& fileId, const string& fileName,
              long fileSize = -1, double totalTime = -1, int channels = -1,
              int sampleRate = -1, double bitRate = -1, int bitsPerSample = -1);

          /**
           * Set appropriates parameter to SoundSystem. This method should call
           * every time when new song start.
           */
          void setSoundSystemParams(const string &fileId);

          // --------------------------------------------------------------------
          // AudioInfo methods
          // --------------------------------------------------------------------

          long getFileSize(const string &fileId);
          double getTotalTime(const string &fileId);
          int getChannels(const string &fileId);
          long getSampleRate(const string &fileId);
          long getBitRate(const string &fileId);
          int getBitsPerSample(const string &fileId);

          // --------------------------------------------------------------------
          // Additional AudioInfo methods
          // --------------------------------------------------------------------

          void setTotalSamples(const string &fileId, double totalSamples);
          double getTotalSamples(const string &fileId);

          void setCurrentSamples(const string &fileId, const double &samples);
          double getCurrentSamples(const string &fileId);

          void setVorbisComment(const string &fileId, const map < string,
              string > &vorbisComments);
          map < string, string > getVorbisComments(const string &fileId);

        private:
          AudioInfo * _audioInfo;
          SoundSystem::SoundSystem * _soundSystem;
      };
    }
  }
}

#endif
