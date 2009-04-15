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
    int AudioProxy::play(char * buf, const int bufSize) {
      return (_soundSystem -> play(buf, bufSize));
    }

    void AudioProxy::setAudioParams(string fileName, long int fileSize,
        double totalTime, int channels, int sampleRate, double bitRate,
        int bitsPerSample) {
      // update information in AudioInfo
      _audioInfo -> setAudioParams(fileName, fileSize, totalTime, channels,
          sampleRate, bitRate, bitsPerSample);
      // set audio parameters for audio in SoundSystem
      _soundSystem -> setAudioParams(channels, sampleRate, bitsPerSample);
    }
  }
}
