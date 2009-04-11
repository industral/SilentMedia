################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SML/Utils/Func/Func.cpp 

OBJS += \
./src/SML/Utils/Func/Func.o 

CPP_DEPS += \
./src/SML/Utils/Func/Func.d 


# Each subdirectory must supply rules for building sources it contributes
src/SML/Utils/Func/%.o: ../src/SML/Utils/Func/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


