################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DSP_utils.c \
../Core/Src/MIDI_application.c \
../Core/Src/MY_CS43L22.c \
../Core/Src/OSC.c \
../Core/Src/envelope.c \
../Core/Src/filter.c \
../Core/Src/fm_modulator.c \
../Core/Src/main.c \
../Core/Src/mixer.c \
../Core/Src/note_frequency.c \
../Core/Src/output_handler.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/timer_utils.c \
../Core/Src/usbh_MIDI.c \
../Core/Src/wavetable.c 

OBJS += \
./Core/Src/DSP_utils.o \
./Core/Src/MIDI_application.o \
./Core/Src/MY_CS43L22.o \
./Core/Src/OSC.o \
./Core/Src/envelope.o \
./Core/Src/filter.o \
./Core/Src/fm_modulator.o \
./Core/Src/main.o \
./Core/Src/mixer.o \
./Core/Src/note_frequency.o \
./Core/Src/output_handler.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/timer_utils.o \
./Core/Src/usbh_MIDI.o \
./Core/Src/wavetable.o 

C_DEPS += \
./Core/Src/DSP_utils.d \
./Core/Src/MIDI_application.d \
./Core/Src/MY_CS43L22.d \
./Core/Src/OSC.d \
./Core/Src/envelope.d \
./Core/Src/filter.d \
./Core/Src/fm_modulator.d \
./Core/Src/main.d \
./Core/Src/mixer.d \
./Core/Src/note_frequency.d \
./Core/Src/output_handler.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/timer_utils.d \
./Core/Src/usbh_MIDI.d \
./Core/Src/wavetable.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../USB_HOST/App -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DSP_utils.d ./Core/Src/DSP_utils.o ./Core/Src/DSP_utils.su ./Core/Src/MIDI_application.d ./Core/Src/MIDI_application.o ./Core/Src/MIDI_application.su ./Core/Src/MY_CS43L22.d ./Core/Src/MY_CS43L22.o ./Core/Src/MY_CS43L22.su ./Core/Src/OSC.d ./Core/Src/OSC.o ./Core/Src/OSC.su ./Core/Src/envelope.d ./Core/Src/envelope.o ./Core/Src/envelope.su ./Core/Src/filter.d ./Core/Src/filter.o ./Core/Src/filter.su ./Core/Src/fm_modulator.d ./Core/Src/fm_modulator.o ./Core/Src/fm_modulator.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mixer.d ./Core/Src/mixer.o ./Core/Src/mixer.su ./Core/Src/note_frequency.d ./Core/Src/note_frequency.o ./Core/Src/note_frequency.su ./Core/Src/output_handler.d ./Core/Src/output_handler.o ./Core/Src/output_handler.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/timer_utils.d ./Core/Src/timer_utils.o ./Core/Src/timer_utils.su ./Core/Src/usbh_MIDI.d ./Core/Src/usbh_MIDI.o ./Core/Src/usbh_MIDI.su ./Core/Src/wavetable.d ./Core/Src/wavetable.o ./Core/Src/wavetable.su

.PHONY: clean-Core-2f-Src

