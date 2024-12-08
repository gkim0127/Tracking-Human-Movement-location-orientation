################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/GGPC/Documents/704P2/COMSYS704/Drivers/BSP/SensorTile/SensorTile.c \
C:/Users/GGPC/Documents/704P2/COMSYS704/Drivers/BSP/SensorTile/SensorTile_bus.c 

C_DEPS += \
./Drivers/BSP/SensorTile/SensorTile.d \
./Drivers/BSP/SensorTile/SensorTile_bus.d 

OBJS += \
./Drivers/BSP/SensorTile/SensorTile.o \
./Drivers/BSP/SensorTile/SensorTile_bus.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/SensorTile/SensorTile.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Drivers/BSP/SensorTile/SensorTile.c Drivers/BSP/SensorTile/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"
Drivers/BSP/SensorTile/SensorTile_bus.o: C:/Users/GGPC/Documents/704P2/COMSYS704/Drivers/BSP/SensorTile/SensorTile_bus.c Drivers/BSP/SensorTile/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -DUSE_STM32L4XX_NUCLEO -DSTM32_SENSORTILE -c -I../../Inc -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/SensorTile -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/Common -I../../../../../../Middlewares/ST/BlueNRG-MS/includes -I../../../../../../Middlewares/ST/BlueNRG-MS/utils -I../../../../../../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -Og -ffunction-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-SensorTile

clean-Drivers-2f-BSP-2f-SensorTile:
	-$(RM) ./Drivers/BSP/SensorTile/SensorTile.cyclo ./Drivers/BSP/SensorTile/SensorTile.d ./Drivers/BSP/SensorTile/SensorTile.o ./Drivers/BSP/SensorTile/SensorTile.su ./Drivers/BSP/SensorTile/SensorTile_bus.cyclo ./Drivers/BSP/SensorTile/SensorTile_bus.d ./Drivers/BSP/SensorTile/SensorTile_bus.o ./Drivers/BSP/SensorTile/SensorTile_bus.su

.PHONY: clean-Drivers-2f-BSP-2f-SensorTile

