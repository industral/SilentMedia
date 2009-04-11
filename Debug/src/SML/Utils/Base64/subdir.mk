################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SML/Utils/Base64/Base64.cpp \
../src/SML/Utils/Base64/test.cpp 

OBJS += \
./src/SML/Utils/Base64/Base64.o \
./src/SML/Utils/Base64/test.o 

CPP_DEPS += \
./src/SML/Utils/Base64/Base64.d \
./src/SML/Utils/Base64/test.d 


# Each subdirectory must supply rules for building sources it contributes
src/SML/Utils/Base64/%.o: ../src/SML/Utils/Base64/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


