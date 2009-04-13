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
  namespace Codec {

    Vorbis::Vorbis(void) :
      dspDev(-1), vi(NULL), length(0), seekPos(0), seek(0) {
      //      this -> ddata = DecodedData::Instance();
    }

    Vorbis::~Vorbis(void) {

    }

    void Vorbis::open(std::string fileName, string fileId) {
      //      this -> dspDev = this -> ddata -> getDSPDev();
      //      this -> dspDev = "default";
      this -> fileName = fileName;

      int openRetVal = ov_fopen(
          const_cast < char* > (this -> fileName.c_str()), &vf);

      if (openRetVal < 0) {
        cerr << "Error to open file " << this -> fileName << endl;
      }

      vi = ov_info(&vf, -1);
      //      this -> ddata -> setMainParam(Utils::Func::getFileSize(
      //          this -> fileName.c_str()), ov_time_total(&vf, -1), vi -> channels,
      //          vi -> rate, vi -> bitrate_nominal, -1);
      this -> readVorbisComment();
    }

    void Vorbis::play(string fileId) {
      if (seek == 0) {
        ov_fopen(const_cast < char* > (this -> fileName.c_str()), &this -> pvf);
      }

      if (ov_seekable(&this -> pvf)) {
        this -> length = ov_time_total(&this -> pvf, -1);
      } else {
        std::cout << "Source is not seekable." << std::endl;
      }

      const int buf_size = 64;
      char buf[buf_size];

      int eof = 0;
      int current_section = 0;


      AO * ao = new AO();
      ao -> init("pulse");
      ao -> setParams(2, 44100, 16);

      while (!eof) {
        long ret = ov_read(&this -> pvf, buf, buf_size, 0, 2, 1,
            &current_section);
        if (ret == 0) {
          eof = 1;
        } else if (ret < 0) {
          std::cout << "ERROR in ov_read()" << std::endl;
        } else {
          ao->play(buf, buf_size);
        }
      }
      ov_clear(&pvf);
    }

    void Vorbis::pause(string fileId) {
      std::cout << "pause" << std::endl;
    }

    void Vorbis::stop(string fileId) {
      std::cout << "stop" << std::endl;
    }

    void Vorbis::close(string fileId) {
      std::cout << "close" << std::endl;
    }

    void Vorbis::setSeek(string fileId, float val) {
      this -> seek = 1;

      int retCode =
          ov_time_seek(&this -> pvf, ((this -> length) * (val / 100)));
      if (retCode != 0) {
        if (retCode == OV_ENOSEEK) {
          std::cout << "Error in ov_time_seek(): Bitstream is not seekable"
              << std::endl;
        } else if (retCode == OV_EINVAL) {
          std::cout
              << "Error in ov_time_seek(): Invalid argument value; possibly called with an OggVorbis_File structure that isn't open"
              << std::endl;
        } else if (retCode == OV_EREAD) {
          std::cout
              << "Error in ov_time_seek(): A read from media returned an error"
              << std::endl;
        } else if (retCode == OV_EFAULT) {
          std::cout
              << "Error in ov_time_seek(): Internal logic fault; indicates a bug or heap/stack corruption."
              << std::endl;
        } else if (retCode == OV_EBADLINK) {
          std::cout
              << "Error in ov_time_seek(): Invalid stream section supplied to libvorbisfile, or the requested link is corrupt."
              << std::endl;
        }
      }
    }

    float Vorbis::getSeek(string fileId) const {
      if (this -> length == 0) {
        return 0;
      } else {
        return (100 / (this -> length / ov_time_tell(
            const_cast < OggVorbis_File* > (&this -> pvf))));
      }
    }

    //    void Vorbis::flush(void) {
    //      this -> seek = 0;
    //    }

    void Vorbis::readVorbisComment(void) {
      //    std::cout << ov_comment ( &vf, -1 ) -> vendor << std::endl;
      char **ptr = ov_comment(&vf, -1) -> user_comments;

      std::vector < std::string > vorbisCommList;

      std::map < std::string, std::string > vorbisComm;
      vorbisComm . clear();

      while (*ptr) {
        std::string myStr = *ptr;
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
