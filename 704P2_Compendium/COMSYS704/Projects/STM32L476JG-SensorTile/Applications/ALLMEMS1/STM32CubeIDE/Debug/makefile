################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/STM32_USBD_Library/Core/subdir.mk
-include Middlewares/STM32_USBD_Library/Class/CDC/subdir.mk
-include Middlewares/BlueNRG-MS/Utils/subdir.mk
-include Middlewares/BlueNRG-MS/HCI/hci_tl_patterns/Basic/subdir.mk
-include Middlewares/BlueNRG-MS/HCI/Controller/subdir.mk
-include Middlewares/BlueNRG-MS/HCI/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/SensorTile/subdir.mk
-include Doc/subdir.mk
-include Application/User/subdir.mk
-include Application/Startup/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := STM32L476JG-SensorTile_ALLMEMS1
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32L476JG-SensorTile_ALLMEMS1.elf \

MAP_FILES += \
STM32L476JG-SensorTile_ALLMEMS1.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32L476JG-SensorTile_ALLMEMS1.list \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: STM32L476JG-SensorTile_ALLMEMS1.elf secondary-outputs

# Tool invocations
STM32L476JG-SensorTile_ALLMEMS1.elf STM32L476JG-SensorTile_ALLMEMS1.map: $(OBJS) $(USER_OBJS) ../STM32L476RGTX.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "STM32L476JG-SensorTile_ALLMEMS1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"../STM32L476RGTX.ld" -Wl,-Map="STM32L476JG-SensorTile_ALLMEMS1.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32L476JG-SensorTile_ALLMEMS1.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32L476JG-SensorTile_ALLMEMS1.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) STM32L476JG-SensorTile_ALLMEMS1.elf STM32L476JG-SensorTile_ALLMEMS1.list STM32L476JG-SensorTile_ALLMEMS1.map default.size.stdout
	-@echo ' '

post-build:
	arm-none-eabi-objcopy -O binary "STM32L476JG-SensorTile_ALLMEMS1.elf" "STM32L476JG-SensorTile_ALLMEMS1.bin"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified post-build

-include ../makefile.targets
