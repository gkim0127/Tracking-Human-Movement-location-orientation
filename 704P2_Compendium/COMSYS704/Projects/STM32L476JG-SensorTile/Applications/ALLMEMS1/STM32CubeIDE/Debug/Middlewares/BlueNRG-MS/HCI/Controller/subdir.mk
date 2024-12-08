################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_gap_aci.c \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_gatt_aci.c \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_hal_aci.c \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_l2cap_aci.c \
C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_utils_small.c 

C_DEPS += \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gap_aci.d \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gatt_aci.d \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_hal_aci.d \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_l2cap_aci.d \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_utils_small.d 

OBJS += \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gap_aci.o \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gatt_aci.o \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_hal_aci.o \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_l2cap_aci.o \
./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_utils_small.o 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gap_aci.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_gap_aci.c Middlewares/BlueNRG-MS/HCI/Controller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gatt_aci.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_gatt_aci.c Middlewares/BlueNRG-MS/HCI/Controller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_hal_aci.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_hal_aci.c Middlewares/BlueNRG-MS/HCI/Controller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_l2cap_aci.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_l2cap_aci.c Middlewares/BlueNRG-MS/HCI/Controller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_utils_small.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Middlewares/ST/BlueNRG-MS/hci/controller/bluenrg_utils_small.c Middlewares/BlueNRG-MS/HCI/Controller/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Middlewares-2f-BlueNRG-2d-MS-2f-HCI-2f-Controller

clean-Middlewares-2f-BlueNRG-2d-MS-2f-HCI-2f-Controller:
	-$(RM) ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gap_aci.cyclo ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gap_aci.d ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gap_aci.o ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gap_aci.su ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gatt_aci.cyclo ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gatt_aci.d ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gatt_aci.o ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_gatt_aci.su ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_hal_aci.cyclo ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_hal_aci.d ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_hal_aci.o ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_hal_aci.su ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_l2cap_aci.cyclo ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_l2cap_aci.d ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_l2cap_aci.o ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_l2cap_aci.su ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_utils_small.cyclo ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_utils_small.d ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_utils_small.o ./Middlewares/BlueNRG-MS/HCI/Controller/bluenrg_utils_small.su

.PHONY: clean-Middlewares-2f-BlueNRG-2d-MS-2f-HCI-2f-Controller

