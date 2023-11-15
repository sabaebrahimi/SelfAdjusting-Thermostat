################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GLCD/ArialFont.c \
../GLCD/ArialFont22pt.c \
../GLCD/ArialFont30pt.c \
../GLCD/AsciiLib.c \
../GLCD/GLCD.c 

OBJS += \
./GLCD/ArialFont.o \
./GLCD/ArialFont22pt.o \
./GLCD/ArialFont30pt.o \
./GLCD/AsciiLib.o \
./GLCD/GLCD.o 

C_DEPS += \
./GLCD/ArialFont.d \
./GLCD/ArialFont22pt.d \
./GLCD/ArialFont30pt.d \
./GLCD/AsciiLib.d \
./GLCD/GLCD.d 


# Each subdirectory must supply rules for building sources it contributes
GLCD/%.o GLCD/%.su: ../GLCD/%.c GLCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xG -c -I../Core/Inc -I../GLCD -I../GUI_Images -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-GLCD

clean-GLCD:
	-$(RM) ./GLCD/ArialFont.d ./GLCD/ArialFont.o ./GLCD/ArialFont.su ./GLCD/ArialFont22pt.d ./GLCD/ArialFont22pt.o ./GLCD/ArialFont22pt.su ./GLCD/ArialFont30pt.d ./GLCD/ArialFont30pt.o ./GLCD/ArialFont30pt.su ./GLCD/AsciiLib.d ./GLCD/AsciiLib.o ./GLCD/AsciiLib.su ./GLCD/GLCD.d ./GLCD/GLCD.o ./GLCD/GLCD.su

.PHONY: clean-GLCD

