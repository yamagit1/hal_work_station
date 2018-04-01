################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/src/stm32f4-hal/stm32f4xx_hal.c \
../hardware/src/stm32f4-hal/stm32f4xx_hal_cortex.c \
../hardware/src/stm32f4-hal/stm32f4xx_hal_dfsdm.c \
../hardware/src/stm32f4-hal/stm32f4xx_hal_flash.c \
../hardware/src/stm32f4-hal/stm32f4xx_hal_gpio.c \
../hardware/src/stm32f4-hal/stm32f4xx_hal_iwdg.c \
../hardware/src/stm32f4-hal/stm32f4xx_hal_pwr.c \
../hardware/src/stm32f4-hal/stm32f4xx_hal_rcc.c 

OBJS += \
./hardware/src/stm32f4-hal/stm32f4xx_hal.o \
./hardware/src/stm32f4-hal/stm32f4xx_hal_cortex.o \
./hardware/src/stm32f4-hal/stm32f4xx_hal_dfsdm.o \
./hardware/src/stm32f4-hal/stm32f4xx_hal_flash.o \
./hardware/src/stm32f4-hal/stm32f4xx_hal_gpio.o \
./hardware/src/stm32f4-hal/stm32f4xx_hal_iwdg.o \
./hardware/src/stm32f4-hal/stm32f4xx_hal_pwr.o \
./hardware/src/stm32f4-hal/stm32f4xx_hal_rcc.o 

C_DEPS += \
./hardware/src/stm32f4-hal/stm32f4xx_hal.d \
./hardware/src/stm32f4-hal/stm32f4xx_hal_cortex.d \
./hardware/src/stm32f4-hal/stm32f4xx_hal_dfsdm.d \
./hardware/src/stm32f4-hal/stm32f4xx_hal_flash.d \
./hardware/src/stm32f4-hal/stm32f4xx_hal_gpio.d \
./hardware/src/stm32f4-hal/stm32f4xx_hal_iwdg.d \
./hardware/src/stm32f4-hal/stm32f4xx_hal_pwr.d \
./hardware/src/stm32f4-hal/stm32f4xx_hal_rcc.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/src/stm32f4-hal/%.o: ../hardware/src/stm32f4-hal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DOS_USE_SEMIHOSTING -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F407xx -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/include" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/include/arm" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/include/cmsis" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/include/cortexm" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/include/diag" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/include/stm32f4-hal" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/include/stm32f4-hal/Legacy" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/src" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/src/cmsis" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/src/cortexm" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/src/diag" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/src/newlib" -I"/home/yama/eclipse-workspace/HAL_WorkStation/hardware/src/stm32f4-hal" -I"/home/yama/eclipse-workspace/HAL_WorkStation/ldscripts" -I"/home/yama/eclipse-workspace/HAL_WorkStation/src" -I"/home/yama/eclipse-workspace/HAL_WorkStation/src/Configure" -I"/home/yama/eclipse-workspace/HAL_WorkStation/src/Peripheral" -std=gnu11 -Wno-bad-function-cast -Wno-conversion -Wno-sign-conversion -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


