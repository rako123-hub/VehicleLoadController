################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CSock.cpp \
../src/C_GPIO.cpp \
../src/LoadControlCalculatur.cpp \
../src/TCPDataClient.cpp \
../src/main.cpp 

OBJS += \
./src/CSock.o \
./src/C_GPIO.o \
./src/LoadControlCalculatur.o \
./src/TCPDataClient.o \
./src/main.o 

CPP_DEPS += \
./src/CSock.d \
./src/C_GPIO.d \
./src/LoadControlCalculatur.d \
./src/TCPDataClient.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C++ Compiler'
	arm-linux-gnueabihf-g++ -mcpu=cortex-a7 -mthumb -mfloat-abi=hard -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -g3 -std=gnu++11 -fabi-version=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


