SilentMedia Library
===================


Features
---------

   * wav, wv, flac, ogg formats support
   * ALSA/OSS/libao
   * OSS Mixer
   * XSPF playlist


INSTALL
-------

libconfig >= 1.4.3: http://www.hyperrealm.com/libconfig/
uriparser >= 0.7.5:  http://uriparser.sourceforge.net/  (./configure --disable-test --disable-doc)
libboost (>= 1.40)

http://xiph.org/downloads/

libxspf >=1.2: http://sourceforge.net/projects/libspiff/files/   (./configure --disable-test)
libflac++-dev
libogg
libvorbis-dev
libao-dev
libasound2-dev (Linux)
libwavpack-dev
liblog4cxx (>= 0.1) http://logging.apache.org/log4cxx/
(for OS X: CXXFLAGS="-stdlib=libstdc++" LDDFLAGS="-stdlib=libstdc++" make)

googletest >= 1.4.0


Application deploy without any problems in Ubuntu 9.10.

To build application:


```bash
cmake .
make
```

for developing:


```bash
cmake -D BuildType=Debug -D LibraryType=shared .
```

for release:


```bash
cmake -D BuildType=Release -D LibraryType=static .
```


```bash
cmake -D Test=true .
cmake -D OSS=1 .
cmake -U OSS .

cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug .
```


### API


```cpp
Audio * audio = new Audio();

audio -> init(); // init Audio system

audio -> open("wav.wav", "file1");

audio -> getInfo("file1");


audio -> play("file1");
audio -> pause("file1");
audio -> play("file1");
audio -> stop("file1");
audio -> close("file1");

audio -> finish();

delete audio; audio = 0;
```

