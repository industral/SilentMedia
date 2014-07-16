#ifndef _SILENTMEDIA_THROW_THROW_HPP_
#define _SILENTMEDIA_THROW_THROW_HPP_

#include <libsml/include.hpp>

using namespace std;

namespace SilentMedia {
  namespace Throw {
    class File {
      public:
        File() {
        }

        File(const string &message) :
          message(message) {
        }

        string getMessage() const {
          return ("Exception: " + this -> message);
        }
      private:
        string message;
    };

    class FileNotFound: public File {
      public:
        FileNotFound() :
          File("File not found") {
        }

        FileNotFound(const string &fileName) :
          File("File not found: " + fileName) {
        }
      private:
    };

    class NotRegularFile: public File {
      public:
        NotRegularFile() :
          File("Not a file") {
        }

        NotRegularFile(const string &fileName) :
          File(fileName + " not a file") {
        }
      private:
    };

    class ErrorOpenFile: public File {
      public:
        ErrorOpenFile() :
          File("Unable to open file") {
        }

        ErrorOpenFile(const string &fileName) :
          File("Unable to open file: " + fileName) {
        }
      private:
    };

    class ErrorCloseFile: public File {
      public:
        ErrorCloseFile() :
          File("Unable to close file") {
        }

        ErrorCloseFile(const string &fileName) :
          File("Unable to close file: " + fileName) {
        }
      private:
    };

  }
}

#endif
