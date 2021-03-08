################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USB_HOST/App/usb_host.c 

OBJS += \
./USB_HOST/App/usb_host.o 

C_DEPS += \
./USB_HOST/App/usb_host.d 


# Each subdirectory must supply rules for building sources it contributes
USB_HOST/App/%.o: ../USB_HOST/App/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F407xx -I"D:/MCU2/1.Start/USB_HOST/App" -I"D:/MCU2/1.Start/USB_HOST/Target" -I"D:/MCU2/1.Start/Core/Inc" -I"D:/MCU2/1.Start/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/MCU2/1.Start/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/MCU2/1.Start/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"D:/MCU2/1.Start/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc" -I"D:/MCU2/1.Start/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/MCU2/1.Start/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


