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

#include <libsml/all.hpp>
#include <libsml/Audio/Audio.hpp>

using namespace SilentMedia;
int main() {
  Audio::Audio * audio = Audio::Audio::Instance();
  audio -> init(); // init Audio system

  Audio::Audio * audio2 = Audio::Audio::Instance();
  audio2 -> init(); // init Audio system

  if (audio -> open("src/test/music/file.ogg", "file1")) {

    //  audio -> getInfo("file1");

    audio -> play("file1");
    //  audio -> pause("file1");
    //  audio -> write("file1");
    //  audio -> stop("file1");
    //  audio -> close("file1");
  }

  delete audio;
  audio = NULL;

  if (audio2 -> open("src/test/music/file.ogg", "file2")) {
    audio2 -> play("file2");
  }

  //  audio -> finish();


  delete audio2;
  audio2 = NULL;

  return 0;
}
