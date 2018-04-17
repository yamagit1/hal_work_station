################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/midleware/lwip/src/core/def.c \
../src/midleware/lwip/src/core/dns.c \
../src/midleware/lwip/src/core/inet_chksum.c \
../src/midleware/lwip/src/core/init.c \
../src/midleware/lwip/src/core/ip.c \
../src/midleware/lwip/src/core/mem.c \
../src/midleware/lwip/src/core/memp.c \
../src/midleware/lwip/src/core/netif.c \
../src/midleware/lwip/src/core/pbuf.c \
../src/midleware/lwip/src/core/raw.c \
../src/midleware/lwip/src/core/stats.c \
../src/midleware/lwip/src/core/sys.c \
../src/midleware/lwip/src/core/tcp.c \
../src/midleware/lwip/src/core/tcp_in.c \
../src/midleware/lwip/src/core/tcp_out.c \
../src/midleware/lwip/src/core/timeouts.c \
../src/midleware/lwip/src/core/udp.c 

OBJS += \
./src/midleware/lwip/src/core/def.o \
./src/midleware/lwip/src/core/dns.o \
./src/midleware/lwip/src/core/inet_chksum.o \
./src/midleware/lwip/src/core/init.o \
./src/midleware/lwip/src/core/ip.o \
./src/midleware/lwip/src/core/mem.o \
./src/midleware/lwip/src/core/memp.o \
./src/midleware/lwip/src/core/netif.o \
./src/midleware/lwip/src/core/pbuf.o \
./src/midleware/lwip/src/core/raw.o \
./src/midleware/lwip/src/core/stats.o \
./src/midleware/lwip/src/core/sys.o \
./src/midleware/lwip/src/core/tcp.o \
./src/midleware/lwip/src/core/tcp_in.o \
./src/midleware/lwip/src/core/tcp_out.o \
./src/midleware/lwip/src/core/timeouts.o \
./src/midleware/lwip/src/core/udp.o 

C_DEPS += \
./src/midleware/lwip/src/core/def.d \
./src/midleware/lwip/src/core/dns.d \
./src/midleware/lwip/src/core/inet_chksum.d \
./src/midleware/lwip/src/core/init.d \
./src/midleware/lwip/src/core/ip.d \
./src/midleware/lwip/src/core/mem.d \
./src/midleware/lwip/src/core/memp.d \
./src/midleware/lwip/src/core/netif.d \
./src/midleware/lwip/src/core/pbuf.d \
./src/midleware/lwip/src/core/raw.d \
./src/midleware/lwip/src/core/stats.d \
./src/midleware/lwip/src/core/sys.d \
./src/midleware/lwip/src/core/tcp.d \
./src/midleware/lwip/src/core/tcp_in.d \
./src/midleware/lwip/src/core/tcp_out.d \
./src/midleware/lwip/src/core/timeouts.d \
./src/midleware/lwip/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
src/midleware/lwip/src/core/%.o: ../src/midleware/lwip/src/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F407xx -I"/home/yama/eclipse-workspace/hal_work_station/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/LCD1202" -I"/home/yama/eclipse-workspace/hal_work_station/src/configure" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs/src/option" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG/source" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/api" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/apps" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/apps/mqtt" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core/ipv4" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core/ipv6" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/apps" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/priv" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/prot" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/netif" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/netif/ppp" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/posix" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/posix/sys" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/netif" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/netif/ppp" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system/arch" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system/OS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/CMSIS_RTOS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/MemMang" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/console" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/cmsi_core" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/hal_driver" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/hal_driver/Legacy" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/start_up" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/stm32f4xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


