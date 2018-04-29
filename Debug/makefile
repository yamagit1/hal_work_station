################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/phy/peripheral_init/subdir.mk
-include src/phy/microSD/subdir.mk
-include src/phy/device/stm32f4xx/subdir.mk
-include src/phy/device/start_up/subdir.mk
-include src/phy/device/hal_driver/subdir.mk
-include src/phy/console/subdir.mk
-include src/phy/LCD1202/subdir.mk
-include src/phy/EthernetENC28J60/subdir.mk
-include src/phy/DriveLed/subdir.mk
-include src/os_kernel/FreeRTOS/Source/portable/MemMang/subdir.mk
-include src/os_kernel/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include src/os_kernel/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include src/os_kernel/FreeRTOS/Source/subdir.mk
-include src/os_kernel/FreeRTOS/subdir.mk
-include src/os_kernel/FatFs/src/option/subdir.mk
-include src/os_kernel/FatFs/src/subdir.mk
-include src/os_kernel/FatFs/subdir.mk
-include src/midleware/network/subdir.mk
-include src/midleware/PM/subdir.mk
-include src/midleware/Monitor/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
hal_work_station.bin \

SECONDARY_SIZE += \
hal_work_station.siz \


# All Target
all: hal_work_station.elf secondary-outputs

# Tool invocations
hal_work_station.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM Cross C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "/home/yama/Desktop/workspace/hal_work_station/linkscript/STM32F407VGTx_FLASH.ld" -Xlinker --gc-sections -Wl,-Map,"hal_work_station.map" --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group -o "hal_work_station.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

hal_work_station.bin: hal_work_station.elf
	@echo 'Invoking: GNU ARM Cross Create Flash Image'
	arm-none-eabi-objcopy -O binary "hal_work_station.elf"  "hal_work_station.bin"
	@echo 'Finished building: $@'
	@echo ' '

hal_work_station.siz: hal_work_station.elf
	@echo 'Invoking: GNU ARM Cross Print Size'
	arm-none-eabi-size --format=berkeley "hal_work_station.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) hal_work_station.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
