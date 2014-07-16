#include "Vorbis.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {
      namespace Codec {

        // --------------------------------------------------------------------
        // Public methods
        // --------------------------------------------------------------------

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
          string fileName = this -> audioProxy -> getFileNameByFileId(fileId);

          if (!boost::filesystem::exists(fileName)) {
            cout << "ERROR: File " << fileName << " not found" << endl;
            return false;
          }

          // try to open file
          int openRetVal = ov_fopen(const_cast < char* > (fileName.c_str()),
              &this -> vorbisFileMap[fileId]);

          // check open result
          if (openRetVal < 0) {
            cerr << "Error to open file \"" << fileName << "\"" << endl;
            //exit(-1);
          }

          // get file information
          this -> vorbisInfoMap[fileId] = ov_info(
              &this -> vorbisFileMap[fileId], -1);

          // update audio information
          this -> audioProxy -> setAudioParams(fileId, fileName,
              boost::filesystem::file_size(fileName), ov_time_total(
                  &this -> vorbisFileMap[fileId], -1),
              this -> vorbisInfoMap[fileId] -> channels,
              this -> vorbisInfoMap[fileId] -> rate,
              this -> vorbisInfoMap[fileId] -> bitrate_nominal, 16);

          // read vorbis comments
          this -> readVorbisComment(fileId);

          return true;
        }

        int Vorbis::play(const string &fileId, bool resume) {
          // set params if it in first time
          if (!resume) {
            // now set parameters to sound system
            this -> audioProxy -> setSoundSystemParams(fileId);
          }

          const int buf_size = 128;
          char buf[buf_size];

          int eof = 0;
          int current_section = 0;

          while (!eof) {
            if (this -> stopMap[fileId]) {
              this -> stopMap[fileId] = false;
              return 0;
            }

            long ret = ov_read(&this -> vorbisFileMap[fileId], buf, buf_size,
                0, 2, 1, &current_section);
            if (ret == 0) {
              eof = 1;
            } else if (ret < 0) {
              cerr << "ERROR in ov_read()" << endl;
            } else {
              this -> audioProxy -> write(buf, buf_size);
            }
          }
          this -> close(fileId);
          return 0;
        }

        void Vorbis::stop(const string &fileId) {
          this -> stopMap[fileId] = true;
        }

        void Vorbis::close(const string &fileId) {
          int retCode = ov_clear(&this -> vorbisFileMap[fileId]);
          if (retCode != 0) {
            cerr << "Error to close file: "
                << this -> audioProxy -> getFileNameByFileId(fileId) << endl;
          }

          // clean vorbis maps
          this -> vorbisFileMap.erase(fileId);
          this -> vorbisInfoMap.erase(fileId);
        }

        void Vorbis::setSeek(const string &fileId, const double &val) {
          int retCode = ov_time_seek(&this -> vorbisFileMap[fileId],
              ((this -> audioProxy -> getTotalTime(fileId)) * (val / 100)));
          if (retCode != 0) {
            if (retCode == OV_ENOSEEK) {
              cerr << "Error in ov_time_seek(): bitstream is not seekable"
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
          if (this -> audioProxy -> getTotalTime(fileId) == 0) {
            return 0;
          } else {
            return (100
                / (this -> audioProxy -> getTotalTime(fileId)
                    / ov_time_tell(
                        const_cast < OggVorbis_File* > (&this -> vorbisFileMap[fileId]))));
          }
        }

        //    void Vorbis::flush(void) {
        //      this -> seek = 0;
        //    }

        // --------------------------------------------------------------------
        // Private methods
        // --------------------------------------------------------------------

        void Vorbis::readVorbisComment(const string &fileId) {
          char **ptr =
              ov_comment(&this -> vorbisFileMap[fileId], -1) -> user_comments;

          vector < string > vorbisCommList;

          map < string, string > vorbisComments;

          while (*ptr) {
            string myStr = *ptr;
            int find = myStr.find_first_of("=");
            if (find) {
              string key = myStr.substr(0, find);
              transform(key.begin(), key.end(), key.begin(), ::toupper);
              string value = myStr.substr(find + 1);

              vorbisComments[key] = value;
            }
            ++ptr;
          }
          this -> audioProxy -> setVorbisComment(fileId, vorbisComments);
        }

      }
    }
  }
}
