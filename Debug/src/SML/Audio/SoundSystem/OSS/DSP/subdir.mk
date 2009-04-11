################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SML/Audio/SoundSystem/OSS/DSP/OSS_DSP.cpp 

OBJS += \
./src/SML/Audio/SoundSystem/OSS/DSP/OSS_DSP.o 

CPP_DEPS += \
./src/SML/Audio/SoundSystem/OSS/DSP/OSS_DSP.d 


# Each subdirectory must supply rules for building sources it contributes
src/SML/Audio/SoundSystem/OSS/DSP/%.o: ../src/SML/Audio/SoundSystem/OSS/DSP/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


