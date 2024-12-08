################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

C_DEPS += \
./Middlewares/STM32_USBD_Library/Core/usbd_core.d \
./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.d \
./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.d 

OBJS += \
./Middlewares/STM32_USBD_Library/Core/usbd_core.o \
./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.o \
./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/STM32_USBD_Library/Core/usbd_core.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c Middlewares/STM32_USBD_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c Middlewares/STM32_USBD_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Middlewares/STM32_USBD_Library/Core/usbd_ioreq.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c Middlewares/STM32_USBD_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Middlewares-2f-STM32_USBD_Library-2f-Core

clean-Middlewares-2f-STM32_USBD_Library-2f-Core:
	-$(RM) ./Middlewares/STM32_USBD_Library/Core/usbd_core.cyclo ./Middlewares/STM32_USBD_Library/Core/usbd_core.d ./Middlewares/STM32_USBD_Library/Core/usbd_core.o ./Middlewares/STM32_USBD_Library/Core/usbd_core.su ./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.cyclo ./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.d ./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.o ./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.su ./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.cyclo ./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.d ./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.o ./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.su

.PHONY: clean-Middlewares-2f-STM32_USBD_Library-2f-Core

