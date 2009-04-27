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

#include "Audio.hpp"

class songPref {
  public:
    string fileId;
    bool resume;
};

// write thread function
void * playThreadFunc(void * data) {
  SilentMedia::Audio::Audio * audio =
      static_cast < SilentMedia::Audio::Audio * > (data);
  audio -> playInThread();
//  pthread_exit(NULL);
  return NULL;
}

namespace SilentMedia {
  namespace Audio {
    Audio * Audio::_audio = NULL;

    Audio::Audio() {
      // create SoundSystem instance
      _soundSystem = SoundSystem::SoundSystem::Instance();
      // create AudioInfo instance
      _audioInfo = AudioInfo::Instance();

      // Create a list of supported file formats
      supportedFormats.push_back("wav");
      supportedFormats.push_back("ogg");
      supportedFormats.push_back("flac");
      supportedFormats.push_back("wv");

      codecHashMap["ogg"] = new Codec::Vorbis();
      codecHashMap["flac"] = new Codec::FLAC();
    }

    Audio::~Audio() {
      delete _soundSystem;
      _soundSystem = NULL;

      delete _audioInfo;
      _audioInfo = NULL;
    }

    Audio * Audio::Instance() {
      if (_audio == NULL) {
        _audio = new Audio();
      }
      return _audio;
    }

    bool Audio::init() {
      return (_soundSystem -> init());
    }

    bool Audio::init(const string &driver) {
      return (_soundSystem -> init(driver));
    }

    void Audio::finish() {
      std::cout << "close audio system" << std::endl;
    }

    bool Audio::open(const string &fileName, const string &fileId) {
      std::cout << "open file name with id: " + fileId << std::endl;

      /*
       * Opening file contains with follow steps:
       * 1. We should check file extension (.ogg, .wav, .flac)
       * 2. Check if file extension equal one of supported file formats.
       * 3. (NOT IMPLEMENTED YET) If this extension is missing, we should find
       * the type of file.
       * 4. If file extension not supported, exit.
       */

      // check if extension is in supportedFormat list
      if (!checkSupportedFormat(fileName)) {
        cerr << "File extension \"" << Utils::Func::getFileExtension(fileName)
            << "\" is unsupported." << endl;
        return false;
      }

      // register fileId with fileName in AudioInfo database
      this -> _audioInfo -> setFileId(fileId, fileName);

      // call open method in appropriate codec
      this -> codecHashMap[Utils::Func::getFileExtension(fileName)] -> open(
          fileId);

      return true;
    }

    void Audio::play(const string &fileId, bool resume) {
      this -> lastFileId = fileId;
      this -> lastResume = resume;

      pthread_create(&this -> threadMap[fileId], NULL, playThreadFunc, this);
      //      threadList.push_back(this -> threadMap[fileId]);
    }

    void Audio::playInThread() {
      this -> playInThread(this -> lastFileId, this -> lastResume);
    }

    void Audio::playInThread(const string &fileId, bool resume) {
      std::cout << "play file name with id: " + fileId << std::endl;
      this -> codecHashMap[Utils::Func::getFileExtension(
          _audioInfo -> getFileNameByFileId(fileId))] -> play(fileId, resume);
    }

    void Audio::pause(const string &fileId) {
      std::cout << "pause file name with id: " + fileId << std::endl;
      pthread_cancel(this -> threadMap[fileId]);
//      pthread_join(this -> threadMap[fileId], NULL);
    }

    void Audio::stop(const string &fileId) {
      if (threadMap[fileId]) {
        cout << "try to stop" << endl;
        pthread_cancel(this -> threadMap[fileId]);
//        pthread_join(this -> threadMap[fileId], NULL);
      }

      std::cout << "stop file name with id: " + fileId << std::endl;
    }

    void Audio::close(const string &fileId) {
      std::cout << "close file name with id: " + fileId << std::endl;
      this -> codecHashMap[Utils::Func::getFileExtension(
          _audioInfo -> getFileNameByFileId(fileId))] -> close(fileId);
    }

    float Audio::getSeek(const string &fileId) {
      return (this -> codecHashMap[Utils::Func::getFileExtension(
          _audioInfo -> getFileNameByFileId(fileId))] -> getSeek(fileId));
    }

    void Audio::setSeek(const string &fileId, const float &seekVal) {
      this -> codecHashMap[Utils::Func::getFileExtension(
          _audioInfo -> getFileNameByFileId(fileId))] -> setSeek(fileId,
          seekVal);
    }

    bool Audio::checkSupportedFormat(const string &fileName) {
      list < string >::iterator it;
      for (it = supportedFormats.begin(); it != supportedFormats.end(); ++it) {
        if (Utils::Func::getFileExtension(fileName).compare(*it)) {
          return true;
        }
        return false;
      }
      return false;
    }

    // AudioInfo methods
    long Audio::getFileSize(const string &fileId) {
      return _audioInfo -> getFileSize(fileId);
    }
    double Audio::getTotalTime(const string &fileId) {
      return _audioInfo -> getTotalTime(fileId);
    }
    int Audio::getChannels(const string &fileId) {
      return _audioInfo -> getChannels(fileId);
    }
    long Audio::getSampleRate(const string &fileId) {
      return _audioInfo -> getSampleRate(fileId);
    }
    long Audio::getBitRate(const string &fileId) {
      return _audioInfo -> getBitRate(fileId);
    }
    int Audio::getBitsPerSample(const string &fileId) {
      return _audioInfo -> getBitsPerSample(fileId);
    }
  }
}
