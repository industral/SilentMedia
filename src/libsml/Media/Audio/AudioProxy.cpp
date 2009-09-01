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

#include "AudioProxy.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      AudioProxy::AudioProxy() {
        // create SoundSystem instance
        _soundSystem = SoundSystem::SoundSystem::Instance();
        // create AudioInfo instance
        _audioInfo = AudioInfo::Instance();
      }

      AudioProxy::~AudioProxy() {
      }

      // AudioInfo methods
      string AudioProxy::getFileNameByFileId(const string& fileId) {
        return (this -> _audioInfo -> getFileNameByFileId(fileId));
      }

      // SoundSystem methods
      int AudioProxy::write(void *buf, const int &bufSize) {
        return (_soundSystem -> write(buf, bufSize));
      }

      void AudioProxy::setAudioParams(const string& fileId,
          const string& fileName, long fileSize, double totalTime,
          int channels, int sampleRate, double bitRate, int bitsPerSample) {
        // update information in AudioInfo
        _audioInfo -> setAudioParams(fileId, fileName, fileSize, totalTime,
            channels, sampleRate, bitRate, bitsPerSample);
      }

      void AudioProxy::setSoundSystemParams(const string &fileId) {
        // set audio parameters for audio in SoundSystem
        _soundSystem -> setAudioParams(_audioInfo -> getChannels(fileId),
            _audioInfo -> getSampleRate(fileId),
            _audioInfo -> getBitsPerSample(fileId));
      }

      // --------------------------------------------------------------------
      // AudioInfo methods
      // --------------------------------------------------------------------

      long AudioProxy::getFileSize(const string &fileId) {
        return (this -> _audioInfo -> getFileSize(fileId));
      }

      double AudioProxy::getTotalTime(const string &fileId) {
        return (this -> _audioInfo -> getTotalTime(fileId));
      }

      int AudioProxy::getChannels(const string &fileId) {
        return (this -> _audioInfo -> getChannels(fileId));
      }

      long AudioProxy::getSampleRate(const string &fileId) {
        return (this -> _audioInfo -> getSampleRate(fileId));
      }

      long AudioProxy::getBitRate(const string &fileId) {
        return (this -> _audioInfo -> getBitRate(fileId));
      }

      int AudioProxy::getBitsPerSample(const string &fileId) {
        return (this -> _audioInfo -> getBitsPerSample(fileId));
      }

      // --------------------------------------------------------------------
      // Additional AudioInfo methods
      // --------------------------------------------------------------------

      void AudioProxy::setTotalSamples(const string &fileId,
          double totalSamples) {
        this -> _audioInfo -> setTotalSamples(fileId, totalSamples);
      }

      double AudioProxy::getTotalSamples(const string &fileId) {
        return (this -> _audioInfo -> getTotalSamples(fileId));
      }

      void AudioProxy::setCurrentSamples(const string &fileId,
          const double &samples) {
        this -> _audioInfo -> setCurrentSamples(fileId, samples);
      }

      double AudioProxy::getCurrentSamples(const string &fileId) {
        return (this -> _audioInfo -> getCurrentSamples(fileId));
      }

      void AudioProxy::setVorbisComment(const string &fileId, const map <
          string, string > &vorbisComments) {
        this -> _audioInfo -> setVorbisComment(fileId, vorbisComments);
      }

      map < string, string > AudioProxy::getVorbisComments(const string &fileId) {
        return this -> _audioInfo -> getVorbisComments(fileId);
      }

    }
  }
}
