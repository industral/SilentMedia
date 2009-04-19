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

#ifndef _SILENTMEDIA_AUDIO_HPP_
#define _SILENTMEDIA_AUDIO_HPP_

// main include
#include <libsml/include.hpp>

// Util class
#include <libsml/Utils/Func/Func.hpp>

/*
 * We should include:
 * SoundSystem, AbstractCodec, AudioInfo.
 */
#include <libsml/Audio/SoundSystem/SoundSystem.hpp>
#include <libsml/Audio/Codec/AbstractCodec.hpp>
#include <libsml/Audio/AudioInfo.hpp>

/*
 * Include all audio codec.
 */
#include <libsml/Audio/Codec/Vorbis/Vorbis.hpp>

// include Boost Thread
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

namespace SilentMedia {
  namespace Audio {
    /**
     * Main audio class. Should start programming from here. Provide main
     * interface to write music, control it and fetch information about it.
     */
    class Audio {
      public:
        /**
         * Constructor.
         */
        Audio();

        /**
         * Destructor.
         */
        ~Audio();

        static Audio * Instance();

        /**
         * Initialized audio system with appropriate sound driver.
         * @param[in] driver audio system driver.
         * @return true in success, false in error.
         * @see http://www.xiph.org/ao/doc/drivers.html
         */
        bool init(const string &driver);
        bool init();

        void finish();

        bool open(const string &fileName, const string &fileId);
        void play(const string &fileId);
        void pause(const string &fileId);
        void stop(const string &fileId);
        void close(const string &fileId);

        float getSeek(const string &fileId);
        void setSeek(const string &fileId, const float &seekVal);

        // AudioInfo methods
        long getFileSize(const string &fileId);
        double getTotalTime(const string &fileId);
        int getChannels(const string &fileId);
        int getSampleRate(const string &fileId);
        double getBitRate(const string &fileId);
        int getBitsPerSample(const string &fileId);
      private:
        // self instance variable
        static Audio * _audio;

        SoundSystem::SoundSystem * _soundSystem;
        AudioInfo * _audioInfo;

        list < string > supportedFormats;
        map < string, Codec::AbstractCodec * > codecHashMap;

        map < string, boost::thread * > threadMap;
        //        list < pthread_t > threadList;

        // file extension
        string fileExt;
        string tmpId;

        bool checkSupportedFormat();
        void playInThread(const string &fileId);
    };
  }
}

#endif
