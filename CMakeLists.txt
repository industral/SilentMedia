cmake_minimum_required(VERSION 2.6)

project (SilentMedia)

set (SourcePath src/libsml)
set(SilentMedia_src

    ${SourcePath}/Utils/Base64/Base64.cpp
    ${SourcePath}/Utils/String/String.cpp

    ${SourcePath}/Media/Container/FileLoader.cpp

    ${SourcePath}/Media/Container/OGG/OGG.cpp

    ${SourcePath}/Media/PlayList/XSPF/XSPF.cpp
    ${SourcePath}/Media/PlayList/XSPF/libXSPF.cpp
    ${SourcePath}/Media/PlayList/PlayList.cpp

    ${SourcePath}/Media/Audio/SoundSystem/OSS/DSP/DSP.cpp
    ${SourcePath}/Media/Audio/SoundSystem/OSS/Mixer/Mixer.cpp
    
    ${SourcePath}/Media/Audio/SoundSystem/ALSA/DSP/DSP.cpp
    ${SourcePath}/Media/Audio/SoundSystem/ALSA/Mixer/Mixer.cpp

    ${SourcePath}/Media/Audio/Audio.cpp
    ${SourcePath}/Media/Audio/AudioInfo.cpp
    ${SourcePath}/Media/Audio/AudioProxy.cpp

    ${SourcePath}/Media/Audio/SoundSystem/SoundSystem.cpp
    ${SourcePath}/Media/Audio/SoundSystem/libao/AO.cpp

    ${SourcePath}/Media/Audio/Codec/WAV/WAV.cpp
    ${SourcePath}/Media/Audio/Codec/Vorbis/Vorbis.cpp
    ${SourcePath}/Media/Audio/Codec/WavPack/WavPack.cpp
    ${SourcePath}/Media/Audio/Codec/FLAC/FLAC.cpp
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
    xspf
    gtest
)

include_directories (
    /usr/include
    /usr/local/include
    /usr/include/c++/4.4
    ${SilentMedia_SOURCE_DIR}/src
    ${SilentMedia_SOURCE_DIR}/${SourcePath}
)

add_library(sml SHARED ${SilentMedia_src})
set_target_properties(sml PROPERTIES CLEAN_DIRECT_OUTPUT 1)

link_directories (${SilentMedia_SOURCE_DIR})

### TEST ###

#add_executable (bin/TestXSPF ${SourcePath}/Test/Media/PlayLists/XSPF/TestXSPF.cpp)
#target_link_libraries (bin/TestXSPF ${SilentMedia_LINKED_LIBRARY})

#add_executable (bin/test1 ${SourcePath}/Test/test.cpp)
#target_link_libraries (bin/test1 ${SilentMedia_LINKED_LIBRARY})

#add_executable (bin/TestFileLoader ${SourcePath}/Test/Media/Container/FileLoader/TestFileLoader.cpp)
#target_link_libraries (bin/TestFileLoader ${SilentMedia_LINKED_LIBRARY})

#add_executable (bin/testMixer ${SourcePath}/Test/testMixer.cpp)
#target_link_libraries (bin/testMixer ${SilentMedia_LINKED_LIBRARY})

### TEST ###

#-pipe -Wall -W -Werror -ggdb
set_target_properties(sml PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb")

ADD_CUSTOM_TARGET(doc COMMAND doxygen ${SilentMedia_SOURCE_DIR}/doc/Doxyfile)
