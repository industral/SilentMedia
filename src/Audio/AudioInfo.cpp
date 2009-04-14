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

#include "AudioInfo.hpp"

namespace SilentMedia {
  namespace Audio {
    AudioInfo * AudioInfo::_info = NULL;

    AudioInfo::AudioInfo() {
      //    ssystem(NULL), playCheck(false) {
      //    this -> ssystem = SoundSystemManager::Instance();

      //    this -> audio = Audio::Instance();
    }

    AudioInfo::~AudioInfo(void) {
      //    cout << "AudioInfo Destructor.. " << endl;
    }

    AudioInfo * AudioInfo::Instance(void) {
      if (_info == NULL) {
        _info = new AudioInfo();
      }
      return _info;
    }

    void AudioInfo::setAudioParams(string fileName, long int fileSize,
        double totalTime, int channels, int sampleRate, double bitRate,
        int bitsPerSample) {
      this -> fileName = fileName;
      this -> fileSize = fileSize;
      this -> totalTime = totalTime;
      this -> channels = channels;
      this -> sampleRate = sampleRate;
      this -> bitRate = bitRate;
      this -> bitsPerSample = bitsPerSample;
    }

    bool AudioInfo::play(const void * buf, string id) {
      //    this -> ssystem -> play(buf, id);
      return true;
    }

  //  void AudioInfo::begin(string id) {
  //    this -> ssystem -> allocateQuery(id);
  //    this -> ssystem -> setParams(this -> channels, this -> sampleRate);
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

  }
}
