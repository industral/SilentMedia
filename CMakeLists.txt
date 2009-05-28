cmake_minimum_required(VERSION 2.6)

project (SilentMedia)

set(SilentMedia_src

    src/Utils/Base64/Base64.cpp

    src/Media/Container/FileLoader.cpp

    src/Media/Container/OGG/OGG.cpp

    src/Media/PlayList/XSPF/XSPF.cpp
    src/Media/PlayList/XSPF/libXSPF.cpp

    src/Media/Audio/SoundSystem/ALSA/DSP/DSP.cpp
    src/Media/Audio/SoundSystem/ALSA/Mixer/Mixer.cpp

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

set(SilentMedia_LINKED_LIBRARY
    sml
    vorbisfile
    FLAC++
    wavpack
    ao
    asound
    boost_thread-mt
    boost_filesystem-mt
    spiff
)

include_directories (/usr/include /usr/include/c++/4.3 ${SilentMedia_SOURCE_DIR}/src)

add_library(sml SHARED ${SilentMedia_src})
set_target_properties(sml PROPERTIES CLEAN_DIRECT_OUTPUT 1)

link_directories (${SilentMedia_SOURCE_DIR})

### Tests ###

add_executable (bin/TestXSPF src/Test/PlayLists/XSPF/TestXSPF.cpp)
target_link_libraries (bin/TestXSPF ${SilentMedia_LINKED_LIBRARY})

### Tests ###

add_executable (bin/test1 src/test/test.cpp)
add_executable (bin/testLoader src/test/testLoader.cpp)
add_executable (bin/testMixer src/test/testMixer.cpp)

target_link_libraries (bin/test1 ${SilentMedia_LINKED_LIBRARY})
target_link_libraries (bin/testLoader ${SilentMedia_LINKED_LIBRARY})
target_link_libraries (bin/testMixer ${SilentMedia_LINKED_LIBRARY})

#-pipe -Wall -W -Werror -ggdb
set_target_properties(sml PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb")
set_target_properties(bin/test1 PROPERTIES COMPILE_FLAGS "-pipe -Wall -W  -ggdb")
