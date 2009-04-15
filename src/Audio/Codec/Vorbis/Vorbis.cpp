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
        this -> vi = NULL;

        // audio parameters
        this -> length = NULL;
        this -> seekPos = NULL;
        this -> seek = false;
      }

      Vorbis::~Vorbis() {
        // release resources
        delete this -> audioProxy;
        this -> audioProxy = NULL;
      }

      bool Vorbis::open(string fileId) {
        // get fileName
        this -> fileName = this -> audioProxy -> getFileNameByFileId(fileId);

        // try to open file
        int openRetVal = ov_fopen(
            const_cast < char* > (this -> fileName.c_str()), &vf);

        // check open result
        if (openRetVal < 0) {
          cerr << "Error to open file \"" << this -> fileName << "\"" << endl;
          exit(-1);
        }

        // get file information
        this -> vi = ov_info(&this -> vf, -1);

        // update audio information
        this -> audioProxy -> setAudioParams(this -> fileName,
            Utils::Func::getFileSize(this -> fileName), ov_time_total(
                &this -> vf, -1), this -> vi -> channels, this -> vi -> rate,
            this -> vi -> bitrate_nominal, 16);

        // read vorbis comments
        this -> readVorbisComment();
      }

      void Vorbis::play(string fileId) {
        if (this -> seek == 0) {
          ov_fopen(const_cast < char* > (this -> fileName.c_str()),
              &this -> tvf);
        }

        if (ov_seekable(&this -> tvf)) {
          this -> length = ov_time_total(&this -> tvf, -1);
        } else {
          cout << "Source is not seekable." << endl;
        }

        const int buf_size = 64;
        char buf[buf_size];

        int eof = 0;
        int current_section = 0;

        while (!eof) {
          long ret = ov_read(&this -> tvf, buf, buf_size, 0, 2, 1,
              &current_section);
          if (ret == 0) {
            eof = 1;
          } else if (ret < 0) {
            cerr << "ERROR in ov_read()" << endl;
          } else {

            this -> audioProxy -> play(buf, buf_size);
          }
        }
        ov_clear(&tvf);
      }

      void Vorbis::pause(string fileId) {
        cout << "pause" << endl;
      }

      void Vorbis::stop(string fileId) {
        cout << "stop" << endl;
      }

      void Vorbis::close(string fileId) {
        cout << "close" << endl;
      }

      void Vorbis::setSeek(string fileId, float val) {
        this -> seek = 1;

        int retCode = ov_time_seek(&this -> tvf, ((this -> length)
            * (val / 100)));
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

      float Vorbis::getSeek(string fileId) {
        if (this -> length == 0) {
          return 0;
        } else {
          return (100 / (this -> length / ov_time_tell(
              const_cast < OggVorbis_File* > (&this -> tvf))));
        }
      }

      //    void Vorbis::flush(void) {
      //      this -> seek = 0;
      //    }

      void Vorbis::readVorbisComment(void) {
        //    cout << ov_comment ( &vf, -1 ) -> vendor << endl;
        char **ptr = ov_comment(&vf, -1) -> user_comments;

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
