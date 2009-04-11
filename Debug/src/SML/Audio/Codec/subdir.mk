################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/SML/Audio/Codec/DecodedData.o 

CPP_SRCS += \
../src/SML/Audio/Codec/DecodedData.cpp 

OBJS += \
./src/SML/Audio/Codec/DecodedData.o 

CPP_DEPS += \
./src/SML/Audio/Codec/DecodedData.d 


# Each subdirectory must supply rules for building sources it contributes
src/SML/Audio/Codec/%.o: ../src/SML/Audio/Codec/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


