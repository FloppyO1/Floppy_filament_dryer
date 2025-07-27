################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Libraries/Display/ssd1306.c \
../Core/Src/Libraries/Display/ssd1306_fonts.c 

OBJS += \
./Core/Src/Libraries/Display/ssd1306.o \
./Core/Src/Libraries/Display/ssd1306_fonts.o 

C_DEPS += \
./Core/Src/Libraries/Display/ssd1306.d \
./Core/Src/Libraries/Display/ssd1306_fonts.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Libraries/Display/%.o Core/Src/Libraries/Display/%.su Core/Src/Libraries/Display/%.cyclo: ../Core/Src/Libraries/Display/%.c Core/Src/Libraries/Display/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G030xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Libraries-2f-Display

clean-Core-2f-Src-2f-Libraries-2f-Display:
	-$(RM) ./Core/Src/Libraries/Display/ssd1306.cyclo ./Core/Src/Libraries/Display/ssd1306.d ./Core/Src/Libraries/Display/ssd1306.o ./Core/Src/Libraries/Display/ssd1306.su ./Core/Src/Libraries/Display/ssd1306_fonts.cyclo ./Core/Src/Libraries/Display/ssd1306_fonts.d ./Core/Src/Libraries/Display/ssd1306_fonts.o ./Core/Src/Libraries/Display/ssd1306_fonts.su

.PHONY: clean-Core-2f-Src-2f-Libraries-2f-Display

