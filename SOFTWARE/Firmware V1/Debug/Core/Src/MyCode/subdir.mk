################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MyCode/buttons.c \
../Core/Src/MyCode/buzzer.c \
../Core/Src/MyCode/display.c \
../Core/Src/MyCode/materials.c \
../Core/Src/MyCode/pid.c \
../Core/Src/MyCode/settings.c 

OBJS += \
./Core/Src/MyCode/buttons.o \
./Core/Src/MyCode/buzzer.o \
./Core/Src/MyCode/display.o \
./Core/Src/MyCode/materials.o \
./Core/Src/MyCode/pid.o \
./Core/Src/MyCode/settings.o 

C_DEPS += \
./Core/Src/MyCode/buttons.d \
./Core/Src/MyCode/buzzer.d \
./Core/Src/MyCode/display.d \
./Core/Src/MyCode/materials.d \
./Core/Src/MyCode/pid.d \
./Core/Src/MyCode/settings.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/MyCode/%.o Core/Src/MyCode/%.su Core/Src/MyCode/%.cyclo: ../Core/Src/MyCode/%.c Core/Src/MyCode/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G030xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-MyCode

clean-Core-2f-Src-2f-MyCode:
	-$(RM) ./Core/Src/MyCode/buttons.cyclo ./Core/Src/MyCode/buttons.d ./Core/Src/MyCode/buttons.o ./Core/Src/MyCode/buttons.su ./Core/Src/MyCode/buzzer.cyclo ./Core/Src/MyCode/buzzer.d ./Core/Src/MyCode/buzzer.o ./Core/Src/MyCode/buzzer.su ./Core/Src/MyCode/display.cyclo ./Core/Src/MyCode/display.d ./Core/Src/MyCode/display.o ./Core/Src/MyCode/display.su ./Core/Src/MyCode/materials.cyclo ./Core/Src/MyCode/materials.d ./Core/Src/MyCode/materials.o ./Core/Src/MyCode/materials.su ./Core/Src/MyCode/pid.cyclo ./Core/Src/MyCode/pid.d ./Core/Src/MyCode/pid.o ./Core/Src/MyCode/pid.su ./Core/Src/MyCode/settings.cyclo ./Core/Src/MyCode/settings.d ./Core/Src/MyCode/settings.o ./Core/Src/MyCode/settings.su

.PHONY: clean-Core-2f-Src-2f-MyCode

