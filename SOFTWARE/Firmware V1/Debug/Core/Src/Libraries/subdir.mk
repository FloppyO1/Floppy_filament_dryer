################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Libraries/temperature.c 

OBJS += \
./Core/Src/Libraries/temperature.o 

C_DEPS += \
./Core/Src/Libraries/temperature.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Libraries/%.o Core/Src/Libraries/%.su Core/Src/Libraries/%.cyclo: ../Core/Src/Libraries/%.c Core/Src/Libraries/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G030xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Libraries

clean-Core-2f-Src-2f-Libraries:
	-$(RM) ./Core/Src/Libraries/temperature.cyclo ./Core/Src/Libraries/temperature.d ./Core/Src/Libraries/temperature.o ./Core/Src/Libraries/temperature.su

.PHONY: clean-Core-2f-Src-2f-Libraries

