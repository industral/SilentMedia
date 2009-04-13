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
  Audio::Audio() {
  }
  Audio::~Audio() {
  }

  void Audio::init(string soundSystem, string dev) {
    std::cout << "in audio init" << std::endl;


  }

  void Audio::finish() {
    std::cout << "close audio system" << std::endl;
  }

  void Audio::open(string fileName, string fileId) {
    std::cout << "open file name with id: " + fileId << std::endl;

    //    Codec::Vorbis * v = new Codec::Vorbis();
    this -> codecMap["vorbis"] = new Codec::Vorbis();
    this -> codecMap["vorbis"] -> open(fileName, fileId);
  }

  void Audio::play(string fileId) {
    std::cout << "play file name with id: " + fileId << std::endl;

    this -> codecMap["vorbis"] -> play(fileId);
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
}
