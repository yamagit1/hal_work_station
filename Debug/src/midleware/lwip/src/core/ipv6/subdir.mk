################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/midleware/lwip/src/core/ipv6/dhcp6.c \
../src/midleware/lwip/src/core/ipv6/ethip6.c \
../src/midleware/lwip/src/core/ipv6/icmp6.c \
../src/midleware/lwip/src/core/ipv6/inet6.c \
../src/midleware/lwip/src/core/ipv6/ip6.c \
../src/midleware/lwip/src/core/ipv6/ip6_addr.c \
../src/midleware/lwip/src/core/ipv6/ip6_frag.c \
../src/midleware/lwip/src/core/ipv6/mld6.c \
../src/midleware/lwip/src/core/ipv6/nd6.c 

OBJS += \
./src/midleware/lwip/src/core/ipv6/dhcp6.o \
./src/midleware/lwip/src/core/ipv6/ethip6.o \
./src/midleware/lwip/src/core/ipv6/icmp6.o \
./src/midleware/lwip/src/core/ipv6/inet6.o \
./src/midleware/lwip/src/core/ipv6/ip6.o \
./src/midleware/lwip/src/core/ipv6/ip6_addr.o \
./src/midleware/lwip/src/core/ipv6/ip6_frag.o \
./src/midleware/lwip/src/core/ipv6/mld6.o \
./src/midleware/lwip/src/core/ipv6/nd6.o 

C_DEPS += \
./src/midleware/lwip/src/core/ipv6/dhcp6.d \
./src/midleware/lwip/src/core/ipv6/ethip6.d \
./src/midleware/lwip/src/core/ipv6/icmp6.d \
./src/midleware/lwip/src/core/ipv6/inet6.d \
./src/midleware/lwip/src/core/ipv6/ip6.d \
./src/midleware/lwip/src/core/ipv6/ip6_addr.d \
./src/midleware/lwip/src/core/ipv6/ip6_frag.d \
./src/midleware/lwip/src/core/ipv6/mld6.d \
./src/midleware/lwip/src/core/ipv6/nd6.d 


# Each subdirectory must supply rules for building sources it contributes
src/midleware/lwip/src/core/ipv6/%.o: ../src/midleware/lwip/src/core/ipv6/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F407xx -I"/home/yama/eclipse-workspace/hal_work_station/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/LCD1202" -I"/home/yama/eclipse-workspace/hal_work_station/src/configure" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/FatFs/src/option" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/LibJPEG/source" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/api" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/apps" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/apps/mqtt" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core/ipv4" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/core/ipv6" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/apps" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/priv" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/lwip/prot" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/netif" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/netif/ppp" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/posix" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/include/posix/sys" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/netif" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/src/netif/ppp" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system/arch" -I"/home/yama/eclipse-workspace/hal_work_station/src/midleware/lwip/system/OS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/CMSIS_RTOS" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/include" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/yama/eclipse-workspace/hal_work_station/src/os_kernel/FreeRTOS/Source/portable/MemMang" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/console" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/cmsi_core" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/hal_driver" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/hal_driver/Legacy" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/start_up" -I"/home/yama/eclipse-workspace/hal_work_station/src/phy/device/stm32f4xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


