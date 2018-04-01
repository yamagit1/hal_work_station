################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/src/diag/Trace.c \
../hardware/src/diag/trace_impl.c 

OBJS += \
./hardware/src/diag/Trace.o \
./hardware/src/diag/trace_impl.o 

C_DEPS += \
./hardware/src/diag/Trace.d \
./hardware/src/diag/trace_impl.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/src/diag/%.o: ../hardware/src/diag/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DOS_USE_SEMIHOSTING -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/include" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/include/arm" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/include/cmsis" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/include/cortexm" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/include/diag" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/include/stm32f4-hal" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/include/stm32f4-hal/Legacy" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/src" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/src/cmsis" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/src/cortexm" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/src/diag" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/src/newlib" -I"/home/yama/Desktop/workspace/HAL_WorkStation/hardware/src/stm32f4-hal" -I"/home/yama/Desktop/workspace/HAL_WorkStation/ldscripts" -I"/home/yama/Desktop/workspace/HAL_WorkStation/src" -I"/home/yama/Desktop/workspace/HAL_WorkStation/src/Configure" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


