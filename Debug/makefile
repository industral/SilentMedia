################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include test/subdir.mk
-include src/SML/subdir.mk
-include src/SML/Utils/Func/subdir.mk
-include src/SML/Utils/Base64/subdir.mk
-include src/SML/Audio/SoundSystem/subdir.mk
-include src/SML/Audio/SoundSystem/OSS/Mixer/subdir.mk
-include src/SML/Audio/SoundSystem/OSS/DSP/subdir.mk
-include src/SML/Audio/SoundSystem/ALSA/DSP/subdir.mk
-include src/SML/Audio/subdir.mk
-include src/SML/Audio/Codec/WavPack/subdir.mk
-include src/SML/Audio/Codec/WAV/subdir.mk
-include src/SML/Audio/Codec/Vorbis/subdir.mk
-include src/SML/Audio/Codec/FLAC/subdir.mk
-include src/SML/Audio/Codec/subdir.mk
-include CMakeFiles/sml.dir/src/SML/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Utils/Func/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Utils/Base64/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Audio/SoundSystem/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Audio/SoundSystem/OSS/Mixer/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Audio/SoundSystem/OSS/DSP/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Audio/SoundSystem/ALSA/DSP/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Audio/Codec/WAV/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Audio/Codec/subdir.mk
-include CMakeFiles/sml.dir/src/SML/Audio/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SilentMedia

# Tool invocations
SilentMedia: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o"SilentMedia" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) SilentMedia
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
