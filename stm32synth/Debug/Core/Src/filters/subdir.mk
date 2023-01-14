################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/filters/filter_highpass.c \
../Core/Src/filters/filter_lowpass.c \
../Core/Src/filters/filter_resonance.c 

OBJS += \
./Core/Src/filters/filter_highpass.o \
./Core/Src/filters/filter_lowpass.o \
./Core/Src/filters/filter_resonance.o 

C_DEPS += \
./Core/Src/filters/filter_highpass.d \
./Core/Src/filters/filter_lowpass.d \
./Core/Src/filters/filter_resonance.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/filters/%.o Core/Src/filters/%.su: ../Core/Src/filters/%.c Core/Src/filters/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-filters

clean-Core-2f-Src-2f-filters:
	-$(RM) ./Core/Src/filters/filter_highpass.d ./Core/Src/filters/filter_highpass.o ./Core/Src/filters/filter_highpass.su ./Core/Src/filters/filter_lowpass.d ./Core/Src/filters/filter_lowpass.o ./Core/Src/filters/filter_lowpass.su ./Core/Src/filters/filter_resonance.d ./Core/Src/filters/filter_resonance.o ./Core/Src/filters/filter_resonance.su

.PHONY: clean-Core-2f-Src-2f-filters

