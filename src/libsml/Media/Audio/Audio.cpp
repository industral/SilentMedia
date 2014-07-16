#include "Audio.hpp"

namespace SilentMedia {
  namespace Media {
    namespace Audio {

      static LoggerPtr logger(Logger::getLogger("SilentMedia::Media::Audio::Audio"));

      // --------------------------------------------------------------------
      // Public methods
      // --------------------------------------------------------------------

      Audio::Audio() {
        // create SoundSystem instance
        _soundSystem = SoundSystem::SoundSystem::Instance();
        // create AudioInfo instance
        _audioInfo = AudioInfo::Instance();

        // Create a list of supported file formats
        supportedFormats.push_back(".wav");
        supportedFormats.push_back(".ogg");
        supportedFormats.push_back(".flac");
        supportedFormats.push_back(".wv");

        codecHashMap[".wav"] = new Codec::WAV();
        codecHashMap[".ogg"] = new Codec::Vorbis();
        codecHashMap[".flac"] = new Codec::FLAC();
        codecHashMap[".wv"] = new Codec::WavPack();
      }

      Audio::~Audio() {
        delete _soundSystem;
        _soundSystem = NULL;

        delete _audioInfo;
        _audioInfo = NULL;
      }

      bool Audio::init(const string soundDriver, const string driver) {
        return (_soundSystem -> init(soundDriver, driver));
      }

      void Audio::finish() {
        LOG4CXX_DEBUG(logger, "Close audio system");
      }

      //TODO: Need to check if id is unique
      bool Audio::open(const string &fileName, string &fileId) {
        LOG4CXX_DEBUG(logger, "Open filename: " << fileName << " with id: " << fileId);

        //      SilentMedia::Audio::Container::FileLoader * fileLoader =
        //          new SilentMedia::Audio::Container::FileLoader();
        //
        //      fileLoader -> open(fileName);
        //
        //      delete fileLoader;

        /*
         * Opening file contains with follow steps:
         * 1. We should check file extension (.ogg, .wav, .flac, .wv)
         * 2. Check if file extension equal one of supported file formats.
         * 3. (NOT IMPLEMENTED YET) If this extension is missing, we should find
         * the type of file.
         * 4. If file extension not supported, exit.
         */

        // check if extension is in supportedFormat list
        if (!checkSupportedFormat(fileName)) {
          LOG4CXX_WARN(logger, "File extension \"" << Path(fileName).extension() << "\" is unsupported.");
          return false;
        }

        // check unique if fileId. If not, generate it.
        if (!this -> _audioInfo -> getFileNameByFileId(fileId).empty()) {
          fileId = SilentMedia::Utils::Random::getRandomId(fileId);
        }

        // register fileId with fileName in AudioInfo database
        this -> _audioInfo -> setFileId(fileId, fileName);

        // call open method in appropriate codec
        this -> getCodec(fileId) -> open(fileId);

        return true;
      }

      void Audio::play(const string &fileId, bool resume) {
        this -> threadMap[fileId] = new boost::thread(boost::bind(&Audio::playInThread, this, fileId, resume));
      }

      void Audio::playInThread(const string &fileId, const bool &resume) {
        LOG4CXX_DEBUG(logger, "Playing file name with id: " << fileId);
        this -> getCodec(fileId) -> play(fileId, resume);
        this -> threadMap[fileId] -> join();
      }

      void Audio::pause(const string &fileId) {
        LOG4CXX_DEBUG(logger, "Pause file name with id " << fileId);
        this -> getCodec(fileId) -> stop(fileId);
      }

      void Audio::stop(const string &fileId) {
        LOG4CXX_DEBUG(logger, "Trying to stop file name with id: " << fileId);
        if (threadMap[fileId]) {
          this -> getCodec(fileId) -> stop(fileId);
          LOG4CXX_DEBUG(logger, "Stopped.");
        } else {
          LOG4CXX_WARN(logger, "Filename with id: " << fileId << "can't be found. Ignore it.");
        }
      }

      void Audio::close(const string &fileId) {
        LOG4CXX_DEBUG(logger, "Close file name with id: " << fileId);
        this -> getCodec(fileId) -> close(fileId);
        this -> _audioInfo -> removeFileId(fileId);
      }

      float Audio::getSeek(const string &fileId) {
        return (this -> getCodec(fileId) -> getSeek(fileId));
      }

      void Audio::setSeek(const string &fileId, const double &seekVal) {
        this -> getCodec(fileId) -> setSeek(fileId, seekVal);
      }

      // AudioInfo methods
      long Audio::getFileSize(const string &fileId) {
        return _audioInfo -> getFileSize(fileId);
      }
      double Audio::getTotalTime(const string &fileId) {
        return _audioInfo -> getTotalTime(fileId);
      }
      int Audio::getChannels(const string &fileId) {
        return _audioInfo -> getChannels(fileId);
      }
      long Audio::getSampleRate(const string &fileId) {
        return _audioInfo -> getSampleRate(fileId);
      }
      long Audio::getBitRate(const string &fileId) {
        return _audioInfo -> getBitRate(fileId);
      }
      int Audio::getBitsPerSample(const string &fileId) {
        return _audioInfo -> getBitsPerSample(fileId);
      }

      void Audio::setVorbisComment(const string &fileId, const map <string, string> &vorbisComments) {
        this -> _audioInfo -> setVorbisComment(fileId, vorbisComments);
      }

      map <string, string> Audio::getVorbisComments(const string &fileId) {
        return this -> _audioInfo -> getVorbisComments(fileId);
      }

      ::PlayStatus Audio::getPlayStatus(const string & fileId) const {
        return this -> _audioInfo -> getPlayStatus(fileId);
      }

      void Audio::setPlayStatus(const string& fileId, const ::PlayStatus & playStatus) {
        this -> _audioInfo -> setPlayStatus(fileId, playStatus);
      }

      // --------------------------------------------------------------------
      // Private methods
      // --------------------------------------------------------------------

      bool Audio::checkSupportedFormat(const string &fileName) {
        foreach(string s, supportedFormats)
              {
                if (Path(fileName).extension().compare(s)) {
                  return true;
                }
              }
        return false;
      }

      string Audio::getExtension(const string &fileId) {
        return (Path(_audioInfo -> getFileNameByFileId(fileId)).extension()).string();
      }

      Codec::AbstractCodec * Audio::getCodec(const string &fileId) {
        return (this -> codecHashMap[this -> getExtension(fileId)]);
      }

    }
  }
}
