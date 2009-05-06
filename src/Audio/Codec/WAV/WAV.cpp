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

#include "WAV.hpp"

namespace SilentMedia {
  namespace Audio {
    namespace Codec {
      WAV::WAV() {
        // create instance for AudioProxy
        this -> audioProxy = new AudioProxy();
      }

      WAV::~WAV() {
      }

      bool WAV::open(const string &fileId) {
        // get fileName
        string fileName = this -> audioProxy -> getFileNameByFileId(fileId);

        if (!boost::filesystem::exists(fileName)) {
          return false;
        }

        this -> wavInfoMap[fileId] = new WAVInfo();

        // open file
        FILE * pFile = fopen(fileName.c_str(), "rb");

        /*
         * Read info into object of class WAVInfo
         * http://ccrma.stanford.edu/CCRMA/Courses/422/projects/WaveFormat/
         */

        fread(this -> wavInfoMap[fileId], sizeof(WAVInfo), 1, pFile);

        // close file
        if (fclose(pFile) != 0) {
          cerr << "Error in WAV::open(): error in fclose()" << endl;
        }

        //       cout << "ChunkID: " << wavinfo -> getChunkID() << endl;
        //       cout << "ChunkSize: " << wavinfo -> getChunkSize() << endl;
        //       cout << "Format: " << wavinfo -> getFormat() << endl;
        //       cout << "Subchunk1ID: " << wavinfo -> getSubchunk1ID() << endl;
        //       cout << "Subchunk1Size: " << wavinfo -> getSubchunk1Size() << endl;
        //       cout << "AudioFormat: " << wavinfo -> getAudioFormat() << endl;
        //       cout << "NumChannels: " << wavinfo -> getNumChannels() << endl;
        //       cout << "SampleRate: " << wavinfo -> getSampleRate() << endl;
        //       cout << "ByteRate: " << wavinfo -> getByteRate() << endl;
        //       cout << "BlockAlign: " << wavinfo -> getBlockAlign() << endl;
        //       cout << "BitsPerSample: " << wavinfo -> getBitsPerSample() << endl;
        //       cout << "Subchunk2ID: " << wavinfo -> getSubchunk2ID() << endl;
        //       cout << "Subchunk2Size: " << wavinfo -> getSubchunk2Size() << endl;

        /*
         * Getting total time by dividing useful audio data (pcm stream) on
         * bitrate (size of data per second).
         * It should be total count of samples dividing on samples per second (sample rate).
         * But here we can't use it, as we miss sample rate.
         */
        double
            totalTime =
                (static_cast < double > (this -> wavInfoMap[fileId] -> getChunkSize())
                    / (this -> wavInfoMap[fileId] -> getByteRate()));

        int bitRate = ((this -> wavInfoMap[fileId] -> getByteRate()) * 8);

        // update audio information
        this -> audioProxy -> setAudioParams(fileId, fileName,
            this -> wavInfoMap[fileId] -> getChunkSize(), totalTime,
            this -> wavInfoMap[fileId] -> getNumChannels(),
            this -> wavInfoMap[fileId] -> getSampleRate(), bitRate,
            this -> wavInfoMap[fileId] -> getBitsPerSample());

        return true;
      }

      int WAV::play(const string &fileId, bool resume) {
        // get fileName
        string fileName = this -> audioProxy -> getFileNameByFileId(fileId);

        // set params if it in first time
        if (!resume) {
          // now set parameters to sound system
          this -> audioProxy -> setSoundSystemParams(fileId);
        }

        const int bufSize = 4096;
        char buf[bufSize];

        this -> inputFDMap[fileId] = new ifstream();
        this -> inputFDMap[fileId] -> open(fileName.c_str(), ifstream::in);

        // make first seek to PCM audio data, skiping technical information
        this -> inputFDMap[fileId] -> seekg(44);

        while (this -> inputFDMap[fileId] -> read(buf, bufSize)) {
          if (this -> stopMap[fileId]) {
            cout << "EXIT" << endl;
            this -> close(fileId);
            return 0;
          }

          this -> offsetPositionMap[fileId] += bufSize;
          this -> audioProxy -> write(buf, bufSize);
        }

        cout << "End stream!" << endl;
        this -> close(fileId);
        return 0;

      }

      void WAV::stop(const string &fileId) {
        this -> stopMap[fileId] = true;
      }

      void WAV::close(const string &fileId) {
        delete this -> wavInfoMap[fileId];
        this -> wavInfoMap[fileId] = NULL;

        delete this -> inputFDMap[fileId];
        this -> inputFDMap[fileId] = NULL;
      }

      float WAV::getSeek(const string &fileId) {
        if (this -> offsetPositionMap[fileId] == 0) {
          return 0;
        }
        return (100.00 / (this -> audioProxy -> getFileSize(fileId)
            / this -> offsetPositionMap[fileId]));
      }

      void WAV::setSeek(const string &fileId, const double &seekVal) {
        long resultPosition = (this -> audioProxy -> getFileSize(fileId)
            * (seekVal / 100));
        /*
         * If resultPosition it not dividing by 2, than we will give a noise.
         * So, check variable if it dividing by 2, if not - increment it.
         */
        if (resultPosition % 2 != 0) {
          ++resultPosition;
        }

        this -> inputFDMap[fileId] -> seekg(resultPosition);
        this -> offsetPositionMap[fileId] = resultPosition;
      }
    }
  }
}

