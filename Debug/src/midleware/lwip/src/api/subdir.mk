################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/midleware/lwip/src/api/api_lib.c \
../src/midleware/lwip/src/api/api_msg.c \
../src/midleware/lwip/src/api/err.c \
../src/midleware/lwip/src/api/netbuf.c \
../src/midleware/lwip/src/api/netdb.c \
../src/midleware/lwip/src/api/netifapi.c \
../src/midleware/lwip/src/api/sockets.c \
../src/midleware/lwip/src/api/tcpip.c 

OBJS += \
./src/midleware/lwip/src/api/api_lib.o \
./src/midleware/lwip/src/api/api_msg.o \
./src/midleware/lwip/src/api/err.o \
./src/midleware/lwip/src/api/netbuf.o \
./src/midleware/lwip/src/api/netdb.o \
./src/midleware/lwip/src/api/netifapi.o \
./src/midleware/lwip/src/api/sockets.o \
./src/midleware/lwip/src/api/tcpip.o 

C_DEPS += \
./src/midleware/lwip/src/api/api_lib.d \
./src/midleware/lwip/src/api/api_msg.d \
./src/midleware/lwip/src/api/err.d \
./src/midleware/lwip/src/api/netbuf.d \
./src/midleware/lwip/src/api/netdb.d \
./src/midleware/lwip/src/api/netifapi.d \
./src/midleware/lwip/src/api/sockets.d \
./src/midleware/lwip/src/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
src/midleware/lwip/src/api/%.o: ../src/midleware/lwip/src/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F407xx -I"/home/yama/eclipse-workspace/hal_work_station/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/LCD1202" -I"/home/yama/eclipse-workspace/hal_work_station/src/configure" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs/src/option" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG/source" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/api" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/apps" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/apps/mqtt" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core/ipv4" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core/ipv6" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/apps" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/priv" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/prot" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/netif" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/netif/ppp" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/posix" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/posix/sys" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/netif" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/netif/ppp" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system/arch" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system/OS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/CMSIS_RTOS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/MemMang" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/console" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/cmsi_core" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/hal_driver" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/hal_driver/Legacy" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/start_up" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/stm32f4xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


