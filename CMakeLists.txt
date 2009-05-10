cmake_minimum_required(VERSION 2.6)

project (SilentMedia)

set(SilentMedia_src

src/Utils/Base64/Base64.cpp

src/Media/Container/FileLoader.cpp

src/Media/Container/OGG/OGG.cpp

src/Media/Audio/Audio.cpp
src/Media/Audio/AudioInfo.cpp
src/Media/Audio/AudioProxy.cpp

src/Media/Audio/SoundSystem/SoundSystem.cpp
src/Media/Audio/SoundSystem/libao/AO.cpp

src/Media/Audio/Codec/WAV/WAV.cpp
src/Media/Audio/Codec/Vorbis/Vorbis.cpp
src/Media/Audio/Codec/WavPack/WavPack.cpp
src/Media/Audio/Codec/FLAC/FLAC.cpp
)
include_directories (/usr/include /usr/include/c++/4.3 ${SilentMedia_SOURCE_DIR}/src)

add_library(sml SHARED ${SilentMedia_src})
set_target_properties(sml PROPERTIES CLEAN_DIRECT_OUTPUT 1)

link_directories (${SilentMedia_SOURCE_DIR})

add_executable (test1 src/test/test.cpp)
add_executable (testLoader src/test/testLoader.cpp)

target_link_libraries (test1 sml vorbisfile FLAC++ wavpack ao boost_thread-mt boost_filesystem-mt)
target_link_libraries (testLoader sml vorbisfile FLAC++ wavpack ao boost_thread-mt boost_filesystem-mt)

#-pipe -Wall -W -Werror -ggdb
set_target_properties(sml PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb")
set_target_properties(test1 PROPERTIES COMPILE_FLAGS "-pipe -Wall -W  -ggdb")
