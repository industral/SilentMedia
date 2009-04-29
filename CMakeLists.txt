cmake_minimum_required(VERSION 2.6)

project (SilentMedia)

set(SilentMedia_src

src/Utils/Base64/Base64.cpp
src/Utils/Func/Func.cpp

src/Audio/Audio.cpp
src/Audio/AudioInfo.cpp
src/Audio/AudioProxy.cpp

src/Audio/SoundSystem/SoundSystem.cpp
src/Audio/SoundSystem/libao/AO.cpp

src/Audio/Codec/WAV/WAV.cpp
src/Audio/Codec/Vorbis/Vorbis.cpp
src/Audio/Codec/WavPack/WavPack.cpp
src/Audio/Codec/FLAC/FLAC.cpp
)
include_directories (/usr/include /usr/include/c++/4.3 ${SilentMedia_SOURCE_DIR}/src)

add_library(sml SHARED ${SilentMedia_src})
set_target_properties(sml PROPERTIES CLEAN_DIRECT_OUTPUT 1)

link_directories (${SilentMedia_SOURCE_DIR})

add_executable (test1 src/test/test.cpp)
add_executable (testFunc src/test/testFunc.cpp)

target_link_libraries (test1 sml vorbisfile FLAC++ wavpack ao boost_thread-mt)
target_link_libraries (testFunc sml vorbisfile FLAC++ wavpack ao boost_thread-mt)

set_target_properties(sml PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb")
set_target_properties(test1 PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb")
