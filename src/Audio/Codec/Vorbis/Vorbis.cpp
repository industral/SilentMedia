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

#include "Vorbis.hpp"

namespace SilentMedia {
  namespace Audio {
    namespace Codec {

      Vorbis::Vorbis() {
        // create instance for AudioProxy
        this -> audioProxy = new AudioProxy();
      }

      Vorbis::~Vorbis() {
        // release resources
        delete this -> audioProxy;
        this -> audioProxy = NULL;
      }

      bool Vorbis::open(const string &fileId) {
        // get fileName
        this -> fileNameMap[fileId]
            = this -> audioProxy -> getFileNameByFileId(fileId);

        // try to open file
        int openRetVal = ov_fopen(
            const_cast < char* > (this -> fileNameMap[fileId].c_str()),
            &this -> vorbisFileMap[fileId]);

        // check open result
        if (openRetVal < 0) {
          cerr << "Error to open file \"" << this -> fileNameMap[fileId]
              << "\"" << endl;
          exit(-1);
        }

        // get file information
        this -> vorbisInfoMap[fileId] = ov_info(&this -> vorbisFileMap[fileId],
            -1);

        // update audio information
        this -> audioProxy -> setAudioParams(fileId,
            this -> fileNameMap[fileId], Utils::Func::getFileSize(
                this -> fileNameMap[fileId]), ov_time_total(
                &this -> vorbisFileMap[fileId], -1),
            this -> vorbisInfoMap[fileId] -> channels,
            this -> vorbisInfoMap[fileId] -> rate,
            this -> vorbisInfoMap[fileId] -> bitrate_nominal, 16);

        // read vorbis comments
        //        this -> readVorbisComment(fileId);
      }

      void Vorbis::play(const string &fileId) {
        // now set parameters to sound system
        this -> audioProxy -> setSoundSystemParams(fileId);

        if (this -> seekMap[fileId] == 0) {
          ov_fopen(const_cast < char* > (this -> fileNameMap[fileId].c_str()),
              &this -> vorbisFileMap[fileId]);
        }

        if (ov_seekable(&this -> vorbisFileMap[fileId])) {
          this -> lengthMap[fileId] = ov_time_total(
              &this -> vorbisFileMap[fileId], -1);
        } else {
          cout << "Source is not seekable." << endl;
        }

        const int buf_size = 64;
        char buf[buf_size];

        int eof = 0;
        int current_section = 0;

        while (!eof) {
          long ret = ov_read(&this -> vorbisFileMap[fileId], buf, buf_size, 0,
              2, 1, &current_section);
          if (ret == 0) {
            eof = 1;
          } else if (ret < 0) {
            cerr << "ERROR in ov_read()" << endl;
          } else {
            this -> audioProxy -> write(*buf, buf_size);
          }
        }
        ov_clear(&this -> vorbisFileMap[fileId]);
      }

      void Vorbis::pause(const string &fileId) {
        cout << "pause" << endl;
      }

      void Vorbis::stop(const string &fileId) {
        cout << "stop" << endl;
      }

      void Vorbis::close(const string &fileId) {
        cout << "close" << endl;
        ov_clear(&this -> vorbisFileMap[fileId]);

        // clean vorbis maps
        this -> vorbisFileMap.erase(fileId);
        this -> vorbisInfoMap.erase(fileId);
      }

      void Vorbis::setSeek(const string &fileId, const float &val) {
        this -> seekMap[fileId] = 1;

        int retCode = ov_time_seek(&this -> vorbisFileMap[fileId],
            ((this -> lengthMap[fileId]) * (val / 100)));
        if (retCode != 0) {
          if (retCode == OV_ENOSEEK) {
            cerr << "Error in ov_time_seek(): Bitstream is not seekable"
                << endl;
          } else if (retCode == OV_EINVAL) {
            cerr
                << "Error in ov_time_seek(): Invalid argument value; possibly called with an OggVorbis_File structure that isn't open"
                << endl;
          } else if (retCode == OV_EREAD) {
            cerr
                << "Error in ov_time_seek(): A read from media returned an error"
                << endl;
          } else if (retCode == OV_EFAULT) {
            cerr
                << "Error in ov_time_seek(): Internal logic fault; indicates a bug or heap/stack corruption."
                << endl;
          } else if (retCode == OV_EBADLINK) {
            cerr
                << "Error in ov_time_seek(): Invalid stream section supplied to libvorbisfile, or the requested link is corrupt."
                << endl;
          }
        }
      }

      float Vorbis::getSeek(const string &fileId) {
        if (this -> lengthMap[fileId] == 0) {
          return 0;
        } else {
          return (100 / (this -> lengthMap[fileId] / ov_time_tell(
              const_cast < OggVorbis_File* > (&this -> vorbisFileMap[fileId]))));
        }
      }

      //    void Vorbis::flush(void) {
      //      this -> seek = 0;
      //    }

      void Vorbis::readVorbisComment(const string &fileId) {
        //    cout << ov_comment ( &vf, -1 ) -> vendor << endl;
        char **ptr =
            ov_comment(&this -> vorbisFileMap[fileId], -1) -> user_comments;

        vector < string > vorbisCommList;

        map < string, string > vorbisComm;
        vorbisComm . clear();

        while (*ptr) {
          string myStr = *ptr;
          int find = myStr . find_first_of("=");
          if (find) {
            //          vorbisComm[Utils::Func::toUpper(myStr . substr(0, find))]
            //              = myStr . substr(find + 1);
          }
          ++ptr;
        }
        //      this -> ddata -> setVorbisComment(vorbisComm);
      }

    }
  }
}
