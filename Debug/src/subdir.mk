################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c 

OBJS += \
./src/main.o 

C_DEPS += \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F407xx -I"/home/yama/Desktop/workspace/hal_work_station/src" -I"/home/yama/Desktop/workspace/hal_work_station/src/app/webserver" -I"/home/yama/Desktop/workspace/hal_work_station/src/configure" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware/FatFs" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware/FatFs/src" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware/FatFs/src/option" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/CMSIS_RTOS" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/include" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/MemMang" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/console" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/cmsi_core" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/hal_driver" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/hal_driver/Legacy" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/start_up" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/stm32f4xx" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/DriveLed" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/EthernetENC28J60" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/LCD1202" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/peripheral_init" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


