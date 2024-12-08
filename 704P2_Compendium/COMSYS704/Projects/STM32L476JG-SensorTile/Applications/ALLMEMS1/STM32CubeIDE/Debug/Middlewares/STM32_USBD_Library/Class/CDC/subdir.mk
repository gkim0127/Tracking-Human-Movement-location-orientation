################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c 

C_DEPS += \
./Middlewares/STM32_USBD_Library/Class/CDC/usbd_cdc.d 

OBJS += \
./Middlewares/STM32_USBD_Library/Class/CDC/usbd_cdc.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/STM32_USBD_Library/Class/CDC/usbd_cdc.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c Middlewares/STM32_USBD_Library/Class/CDC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Middlewares-2f-STM32_USBD_Library-2f-Class-2f-CDC

clean-Middlewares-2f-STM32_USBD_Library-2f-Class-2f-CDC:
	-$(RM) ./Middlewares/STM32_USBD_Library/Class/CDC/usbd_cdc.cyclo ./Middlewares/STM32_USBD_Library/Class/CDC/usbd_cdc.d ./Middlewares/STM32_USBD_Library/Class/CDC/usbd_cdc.o ./Middlewares/STM32_USBD_Library/Class/CDC/usbd_cdc.su

.PHONY: clean-Middlewares-2f-STM32_USBD_Library-2f-Class-2f-CDC

