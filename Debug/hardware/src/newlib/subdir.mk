################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/src/newlib/_exit.c \
../hardware/src/newlib/_sbrk.c \
../hardware/src/newlib/_startup.c \
../hardware/src/newlib/_syscalls.c \
../hardware/src/newlib/assert.c 

CPP_SRCS += \
../hardware/src/newlib/_cxx.cpp 

OBJS += \
./hardware/src/newlib/_cxx.o \
./hardware/src/newlib/_exit.o \
./hardware/src/newlib/_sbrk.o \
./hardware/src/newlib/_startup.o \
./hardware/src/newlib/_syscalls.o \
./hardware/src/newlib/assert.o 

C_DEPS += \
./hardware/src/newlib/_exit.d \
./hardware/src/newlib/_sbrk.d \
./hardware/src/newlib/_startup.d \
./hardware/src/newlib/_syscalls.d \
./hardware/src/newlib/assert.d 

CPP_DEPS += \
./hardware/src/newlib/_cxx.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/src/newlib/%.o: ../hardware/src/newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DOS_USE_SEMIHOSTING -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/arm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/cmsis" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/cortexm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/diag" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/stm32f4-hal" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/stm32f4-hal/Legacy" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/cmsis" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/cortexm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/diag" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/newlib" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/stm32f4-hal" -I"/home/yama/Desktop/WS/HAL_WorkStation/ldscripts" -I"/home/yama/Desktop/WS/HAL_WorkStation/src" -I"/home/yama/Desktop/WS/HAL_WorkStation/src/Configure" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

hardware/src/newlib/%.o: ../hardware/src/newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DOS_USE_SEMIHOSTING -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/arm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/cmsis" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/cortexm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/diag" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/stm32f4-hal" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/stm32f4-hal/Legacy" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/cmsis" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/cortexm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/diag" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/newlib" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/stm32f4-hal" -I"/home/yama/Desktop/WS/HAL_WorkStation/ldscripts" -I"/home/yama/Desktop/WS/HAL_WorkStation/src" -I"/home/yama/Desktop/WS/HAL_WorkStation/src/Configure" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

hardware/src/newlib/_startup.o: ../hardware/src/newlib/_startup.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DOS_USE_SEMIHOSTING -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -DOS_INCLUDE_STARTUP_INIT_MULTIPLE_RAM_SECTIONS -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/arm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/cmsis" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/cortexm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/diag" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/stm32f4-hal" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/include/stm32f4-hal/Legacy" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/cmsis" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/cortexm" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/diag" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/newlib" -I"/home/yama/Desktop/WS/HAL_WorkStation/hardware/src/stm32f4-hal" -I"/home/yama/Desktop/WS/HAL_WorkStation/ldscripts" -I"/home/yama/Desktop/WS/HAL_WorkStation/src" -I"/home/yama/Desktop/WS/HAL_WorkStation/src/Configure" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"hardware/src/newlib/_startup.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


