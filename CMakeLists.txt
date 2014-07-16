cmake_minimum_required(VERSION 2.6)

PROJECT (SilentMedia)

SET(SourcePath src/libsml)
add_definitions( -DLIBAO=1 )

IF (DEFINED OSS)
SET(OSS_src
    ${SourcePath}/Media/Audio/SoundSystem/OSS/DSP/DSP.cpp
    ${SourcePath}/Media/Audio/SoundSystem/OSS/Mixer/Mixer.cpp
)
ENDIF(DEFINED OSS)

IF (DEFINED ALSA)
SET(ALSA_src
    ${SourcePath}/Media/Audio/SoundSystem/ALSA/DSP/DSP.cpp
    ${SourcePath}/Media/Audio/SoundSystem/ALSA/Mixer/Mixer.cpp
)
ENDIF(DEFINED ALSA)

IF (DEFINED LIBAO)
SET(LIBAO_src
    ${SourcePath}/Media/Audio/SoundSystem/libao/AO.cpp
)
ENDIF(DEFINED LIBAO)

link_directories(/usr/local/lib /usr/local/Cellar/boost/1.55.0_2/lib /usr/local/Cellar/log4cxx/0.10.0/lib)

SET(SilentMedia_src

    ${SourcePath}/Utils/Base64/Base64.cpp
    ${SourcePath}/Utils/String/String.cpp
    ${SourcePath}/Utils/Random/Random.cpp

    ${SourcePath}/Media/Container/FileLoader.cpp

    ${SourcePath}/Media/Container/OGG/OGG.cpp

    ${SourcePath}/Media/PlayList/XSPF/XSPF.cpp
    ${SourcePath}/Media/PlayList/XSPF/libXSPF.cpp
    ${SourcePath}/Media/PlayList/PlayList.cpp

    ${OSS_src}
    ${ALSA_src}
    ${LIBAO_src}

    ${SourcePath}/Media/Audio/Audio.cpp
    ${SourcePath}/Media/Audio/AudioInfo.cpp
    ${SourcePath}/Media/Audio/AudioProxy.cpp

    ${SourcePath}/Media/Audio/SoundSystem/SoundSystem.cpp


    ${SourcePath}/Media/Audio/Codec/WAV/WAV.cpp
    ${SourcePath}/Media/Audio/Codec/Vorbis/Vorbis.cpp
    ${SourcePath}/Media/Audio/Codec/WavPack/WavPack.cpp
    ${SourcePath}/Media/Audio/Codec/FLAC/FLAC.cpp
)

SET(SilentMedia_LINKED_LIBRARY
    vorbisfile
    FLAC++
    wavpack
    ao
    log4cxx
    #asound
    boost_thread-mt
    boost_filesystem-mt
    boost_system-mt
    xspf
    #gtest
)

INCLUDE_DIRECTORIES(
    /usr/include
    /usr/local/include
    ${SilentMedia_SOURCE_DIR}/src
    ${SilentMedia_SOURCE_DIR}/${SourcePath}
)

IF(LibraryType STREQUAL "static")
  ADD_LIBRARY(sml-static STATIC ${SilentMedia_src})
  # rename library from libsml-static.a => libsml.a
  SET_TARGET_PROPERTIES(sml-static PROPERTIES OUTPUT_NAME "sml")
  SET_TARGET_PROPERTIES(sml-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
ELSEIF(LibraryType STREQUAL "shared")
  ADD_LIBRARY(sml SHARED ${SilentMedia_src})
  target_link_libraries (sml ${SilentMedia_LINKED_LIBRARY})

  
  # change compile optimization/debug flags # -Werror -pedantic
  IF(BuildType STREQUAL "Debug")
    SET_TARGET_PROPERTIES(sml PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb")
  ELSEIF(BuildType STREQUAL "Release")
    SET_TARGET_PROPERTIES(sml PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -O3 -fomit-frame-pointer")
  ENDIF()
  
  SET_TARGET_PROPERTIES(sml PROPERTIES CLEAN_DIRECT_OUTPUT 1)
ENDIF()

### TEST ###

IF(Test STREQUAL "true")
  ADD_EXECUTABLE (bin/TestXSPF ${SourcePath}/Test/Media/PlayLists/XSPF/TestXSPF.cpp)
  TARGET_LINK_LIBRARIES (bin/TestXSPF ${SilentMedia_LINKED_LIBRARY})

  ADD_EXECUTABLE (bin/test1 ${SourcePath}/Test/test.cpp)
  TARGET_LINK_LIBRARIES (bin/test1 ${SilentMedia_LINKED_LIBRARY})

  ADD_EXECUTABLE (bin/TestFileLoader ${SourcePath}/Test/Media/Container/FileLoader/TestFileLoader.cpp)
  TARGET_LINK_LIBRARIES (bin/TestFileLoader ${SilentMedia_LINKED_LIBRARY})

  ADD_EXECUTABLE (bin/testMixer ${SourcePath}/Test/testMixer.cpp)
  TARGET_LINK_LIBRARIES (bin/testMixer ${SilentMedia_LINKED_LIBRARY})
ENDIF (Test STREQUAL "true")

### TEST ###

ADD_CUSTOM_TARGET(doc COMMAND doxygen ${SilentMedia_SOURCE_DIR}/doc/Doxyfile)
