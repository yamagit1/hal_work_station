################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/os_kernel/FreeRTOS/Source/croutine.c \
../src/os_kernel/FreeRTOS/Source/event_groups.c \
../src/os_kernel/FreeRTOS/Source/list.c \
../src/os_kernel/FreeRTOS/Source/queue.c \
../src/os_kernel/FreeRTOS/Source/tasks.c \
../src/os_kernel/FreeRTOS/Source/timers.c 

OBJS += \
./src/os_kernel/FreeRTOS/Source/croutine.o \
./src/os_kernel/FreeRTOS/Source/event_groups.o \
./src/os_kernel/FreeRTOS/Source/list.o \
./src/os_kernel/FreeRTOS/Source/queue.o \
./src/os_kernel/FreeRTOS/Source/tasks.o \
./src/os_kernel/FreeRTOS/Source/timers.o 

C_DEPS += \
./src/os_kernel/FreeRTOS/Source/croutine.d \
./src/os_kernel/FreeRTOS/Source/event_groups.d \
./src/os_kernel/FreeRTOS/Source/list.d \
./src/os_kernel/FreeRTOS/Source/queue.d \
./src/os_kernel/FreeRTOS/Source/tasks.d \
./src/os_kernel/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/os_kernel/FreeRTOS/Source/%.o: ../src/os_kernel/FreeRTOS/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F407xx -I"/home/yama/Desktop/workspace/hal_work_station/src" -I"/home/yama/Desktop/workspace/hal_work_station/src/app" -I"/home/yama/Desktop/workspace/hal_work_station/src/app/fptserver" -I"/home/yama/Desktop/workspace/hal_work_station/src/app/webserver" -I"/home/yama/Desktop/workspace/hal_work_station/src/configure" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware/CM" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware/Monitor" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware/network" -I"/home/yama/Desktop/workspace/hal_work_station/src/midleware/PM" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FatFs" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FatFs/src" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FatFs/src/option" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/CMSIS_RTOS" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/include" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/yama/Desktop/workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/MemMang" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/console" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/cmsi_core" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/hal_driver" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/hal_driver/Legacy" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/start_up" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/device/stm32f4xx" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/DriveLed" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/EthernetENC28J60" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/LCD1202" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/microSD" -I"/home/yama/Desktop/workspace/hal_work_station/src/phy/peripheral_init" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


