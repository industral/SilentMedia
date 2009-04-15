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

namespace SilentMedia {
  namespace Audio {
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
    }

    Audio::~Audio() {
      delete _soundSystem;
      _soundSystem = NULL;

      delete _audioInfo;
      _audioInfo = NULL;
    }

    bool Audio::init(string driver) {
      std::cout << "in audio init" << std::endl;
      return (_soundSystem -> init(driver));
      //    ao -> setAudioParams(2, 44100, 16);
    }

    void Audio::finish() {
      std::cout << "close audio system" << std::endl;
    }

    bool Audio::open(string fileName, string fileId) {
      std::cout << "open file name with id: " + fileId << std::endl;

      /*
       * Opening file contains with follow steps:
       * 1. We should check file extension (.ogg, .wav, .flac)
       * 2. Check if file extension equal one of supported file formats.
       * 3. (NOT IMPLEMENTED YET) If this extension is missing, we should find
       * the type of file.
       * 4. If file extension not supported, exit.
       */

      // get file extension
      this -> fileExt = Utils::Func::getFileExtension(fileName);

      // check if extension is in supportedFormat list
      if (!checkSupportedFormat()) {
        cerr << "File extension \"" << fileExt << "\" is unsupported." << endl;
        return false;
      }

      // register fileId with fileName in AudioInfo database
      this -> _audioInfo -> setFileId(fileId, fileName);

      // call open method in appropriate codec
      this -> codecHashMap[this -> fileExt] -> open(fileId);

      return true;
    }

    void Audio::play(string fileId) {
      std::cout << "play file name with id: " + fileId << std::endl;

      codecHashMap[fileExt] -> play(fileId);
    }

    void Audio::pause(string fileId) {
      std::cout << "pause file name with id: " + fileId << std::endl;
    }

    void Audio::stop(string fileId) {
      std::cout << "stop file name with id: " + fileId << std::endl;
    }

    void Audio::close(string fileId) {
      std::cout << "close file name with id: " + fileId << std::endl;
    }

    float Audio::getSeek(string fileId) {
      std::cout << "get seek.. " << std::endl;
      return 0.0;
    }

    void Audio::setSeek(string fileId, float seekVal) {
      std::cout << "set seek value " << seekVal << std::endl;
    }

    bool Audio::checkSupportedFormat(void) {
      list < string >::iterator it;
      for (it = supportedFormats.begin(); it != supportedFormats.end(); ++it) {
        if (fileExt.compare(*it)) {
          return true;
        }
        return false;
      }
    }
  }
}
