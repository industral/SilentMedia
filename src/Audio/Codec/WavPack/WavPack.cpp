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

#include "WavPack.hpp"

namespace SilentMedia {
  namespace Audio {
    namespace Codec {
      WavPack::WavPack() {

        // create instance for AudioProxy
        this -> audioProxy = new AudioProxy();
      }

      WavPack::~WavPack() {
        // release resources
        delete this -> audioProxy;
        this -> audioProxy = NULL;
      }

      bool WavPack::open(const string &fileId) {
        // get fileName
        string fileName = this -> audioProxy -> getFileNameByFileId(fileId);

        if (!boost::filesystem::exists(fileName)) {
          return false;
        }

        char * error = NULL;

        this -> wavPackContextMap[fileId] = WavpackOpenFileInput(
            fileName.c_str(), error, OPEN_WVC, 0);

        if (this -> wavPackContextMap[fileId] == NULL) {
          cerr << "Error in error WavpackOpenFileInput(): " << error << endl;
        }

        long totalTime =
            WavpackGetNumSamples(this -> wavPackContextMap[fileId])
                / WavpackGetSampleRate(this -> wavPackContextMap[fileId]);

        // update audio information
        this -> audioProxy -> setAudioParams(fileId, fileName,
            WavpackGetFileSize(this -> wavPackContextMap[fileId]), totalTime,
            WavpackGetNumChannels(this -> wavPackContextMap[fileId]),
            WavpackGetSampleRate(this -> wavPackContextMap[fileId]),
            WavpackGetAverageBitrate(this -> wavPackContextMap[fileId], 0),
            WavpackGetBitsPerSample(this -> wavPackContextMap[fileId]));

        //    this -> readVorbisComment();

        return true;
      }

      int WavPack::play(const string &fileId, bool resume) {
        // set params if it in first time
        if (!resume) {
          // now set parameters to sound system
          this -> audioProxy -> setSoundSystemParams(fileId);
        }

        const int bufSize = 4096;

        int32_t *buffer = new int32_t[bufSize];
        int16_t *outputBuf = new int16_t[bufSize];

        while ( /*int samplesUnpacked =*/WavpackUnpackSamples(
            this -> wavPackContextMap[fileId], buffer, 1024)) {

          if (this -> stopMap[fileId]) {
            this -> close(fileId);
            return 0;
          }

          for (int i = 0; i < bufSize; ++i) {
            outputBuf[i] = buffer[i];
          }
          this -> audioProxy -> write(outputBuf, bufSize);
        }

        // release resource
        delete[] buffer;
        delete[] outputBuf;

        this -> close(fileId);
        return 0;
      }

      void WavPack::stop(const string &fileId) {
        this -> stopMap[fileId] = true;
      }

      void WavPack::setSeek(const string &fileId, const double &seekVal) {
        uint32_t sample = ((WavpackGetNumSamples(
            this -> wavPackContextMap[fileId])) * (seekVal / 100));

        if (WavpackSeekSample(this -> wavPackContextMap[fileId], sample)
            == false) {
          cerr << "Error in WavpackSeekSample()" << endl;
          this -> close(fileId);
        }
      }

      float WavPack::getSeek(const string &fileId) {
        if (WavpackGetSampleIndex(this -> wavPackContextMap[fileId]) == 0) {
          return 0;
        } else {
          return (100 / (static_cast < double > (WavpackGetNumSamples(
              this -> wavPackContextMap[fileId])) / WavpackGetSampleIndex(
              this -> wavPackContextMap[fileId])));
        }
      }

      void WavPack::close(const string &fileId) {
        WavpackCloseFile(this -> wavPackContextMap[fileId]);
      }

    }
  }
}
