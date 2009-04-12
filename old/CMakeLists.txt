# project (libssoss)
# set(libssoss_src
# src/utils/base64/base64.cpp src/utils/func/func.cpp
# src/audio/oss/dsp/dsp.cpp src/audio/oss/mixer/mixer.cpp
# src/audio/format/wav/wav.cpp src/audio/format/flac/flac.cpp src/audio/format/vorbis/vorbis.cpp src/audio/format/wavpack/wavpack.cpp
# )
# include_directories (${libssoss_SOURCE_DIR}/src ${libssoss_SOURCE_DIR}/src/audio ${libssoss_SOURCE_DIR}/src/audio/oss/dsp /usr/include/ImageMagick)
# 
# add_library(ssoss SHARED ${libssoss_src})
# # add_library(ssoss-static STATIC ${libssoss_src})
# 
# set_target_properties(ssoss PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb -O2")
# # -fomit-frame-pointer -O3 -march=prescott
# set_target_properties(ssoss PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# # # set_target_properties(ssoss-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
cmake_minimum_required(VERSION 2.6)

project (SilentMedia)
set(SilentMedia_src
src/SML/config.cpp

src/SML/Utils/Func/Func.cpp
src/SML/Utils/Base64/Base64.cpp

src/SML/Audio/PAI.cpp

#src/SML/Audio/SoundSystem/OSS/DSP/OSS_DSP.cpp
#src/SML/Audio/SoundSystem/OSS/Mixer/OSS_Mixer.cpp

src/SML/Audio/SoundSystem/ALSA/DSP/ALSA_DSP.cpp

src/SML/Audio/Codec/DecodedData.cpp
src/SML/Audio/SoundSystem/SoundSystemManager.cpp

src/SML/Audio/Audio.cpp
src/SML/Audio/Codec/WAV/WAV.cpp
#src/SML/Audio/Codec/FLAC/FLAC.cpp
#src/SML/Audio/Codec/Vorbis/Vorbis.cpp
#src/SML/Audio/Codec/WavPack/WavPack.cpp
)
include_directories (${SilentMedia_SOURCE_DIR}/src ${SilentMedia_SOURCE_DIR}/src/SML/Audio/Codec ${SilentMedia_SOURCE_DIR}/src/SML/Audio/Codec/WAV  /usr/include/ImageMagick)

add_executable (test1 src/test/test.cpp)
target_link_libraries (test1 sml lua5.1)

add_library(sml SHARED ${SilentMedia_src})
# add_library(ssoss-static STATIC ${libssoss_src})

#set_target_properties(sml PROPERTIES COMPILE_FLAGS "-pipe -Wall -W -ggdb -finline-functions -O2")
# -fomit-frame-pointer -O3 -march=prescott
set_target_properties(sml PROPERTIES CLEAN_DIRECT_OUTPUT 1)
# # set_target_properties(ssoss-static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
