cmake_minimum_required(VERSION 2.6)

project (SilentMedia)

set(SilentMedia_src

src/Audio/Audio.cpp
src/Audio/SoundSystem/libao/AO.cpp
src/Audio/Codec/Vorbis/Vorbis.cpp
)
include_directories (/usr/include /usr/include/c++/4.3 ${SilentMedia_SOURCE_DIR}/src)

add_library(sml SHARED ${SilentMedia_src})
set_target_properties(sml PROPERTIES CLEAN_DIRECT_OUTPUT 1)

link_directories (${SilentMedia_SOURCE_DIR})
add_executable (test1 src/test/test.cpp)
target_link_libraries (test1 sml vorbisfile ao)

set_target_properties(sml PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb")
set_target_properties(test1 PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb")
