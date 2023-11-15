################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GUI_Images/Common/MainBG.c \
../GUI_Images/Common/arrowdown.c \
../GUI_Images/Common/arrowup.c 

OBJS += \
./GUI_Images/Common/MainBG.o \
./GUI_Images/Common/arrowdown.o \
./GUI_Images/Common/arrowup.o 

C_DEPS += \
./GUI_Images/Common/MainBG.d \
./GUI_Images/Common/arrowdown.d \
./GUI_Images/Common/arrowup.d 


# Each subdirectory must supply rules for building sources it contributes
GUI_Images/Common/%.o GUI_Images/Common/%.su: ../GUI_Images/Common/%.c GUI_Images/Common/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xG -c -I../Core/Inc -I../GLCD -I../GUI_Images -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-GUI_Images-2f-Common

clean-GUI_Images-2f-Common:
	-$(RM) ./GUI_Images/Common/MainBG.d ./GUI_Images/Common/MainBG.o ./GUI_Images/Common/MainBG.su ./GUI_Images/Common/arrowdown.d ./GUI_Images/Common/arrowdown.o ./GUI_Images/Common/arrowdown.su ./GUI_Images/Common/arrowup.d ./GUI_Images/Common/arrowup.o ./GUI_Images/Common/arrowup.su

.PHONY: clean-GUI_Images-2f-Common

