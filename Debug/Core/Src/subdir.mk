################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ecat_app.c \
../Core/Src/ecat_slv.c \
../Core/Src/esc.c \
../Core/Src/esc_coe.c \
../Core/Src/esc_eep.c \
../Core/Src/esc_eoe.c \
../Core/Src/esc_foe.c \
../Core/Src/esc_hw.c \
../Core/Src/main.c \
../Core/Src/objectlist.c \
../Core/Src/spi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/ecat_app.o \
./Core/Src/ecat_slv.o \
./Core/Src/esc.o \
./Core/Src/esc_coe.o \
./Core/Src/esc_eep.o \
./Core/Src/esc_eoe.o \
./Core/Src/esc_foe.o \
./Core/Src/esc_hw.o \
./Core/Src/main.o \
./Core/Src/objectlist.o \
./Core/Src/spi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/ecat_app.d \
./Core/Src/ecat_slv.d \
./Core/Src/esc.d \
./Core/Src/esc_coe.d \
./Core/Src/esc_eep.d \
./Core/Src/esc_eoe.d \
./Core/Src/esc_foe.d \
./Core/Src/esc_hw.d \
./Core/Src/main.d \
./Core/Src/objectlist.d \
./Core/Src/spi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ecat_app.cyclo ./Core/Src/ecat_app.d ./Core/Src/ecat_app.o ./Core/Src/ecat_app.su ./Core/Src/ecat_slv.cyclo ./Core/Src/ecat_slv.d ./Core/Src/ecat_slv.o ./Core/Src/ecat_slv.su ./Core/Src/esc.cyclo ./Core/Src/esc.d ./Core/Src/esc.o ./Core/Src/esc.su ./Core/Src/esc_coe.cyclo ./Core/Src/esc_coe.d ./Core/Src/esc_coe.o ./Core/Src/esc_coe.su ./Core/Src/esc_eep.cyclo ./Core/Src/esc_eep.d ./Core/Src/esc_eep.o ./Core/Src/esc_eep.su ./Core/Src/esc_eoe.cyclo ./Core/Src/esc_eoe.d ./Core/Src/esc_eoe.o ./Core/Src/esc_eoe.su ./Core/Src/esc_foe.cyclo ./Core/Src/esc_foe.d ./Core/Src/esc_foe.o ./Core/Src/esc_foe.su ./Core/Src/esc_hw.cyclo ./Core/Src/esc_hw.d ./Core/Src/esc_hw.o ./Core/Src/esc_hw.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/objectlist.cyclo ./Core/Src/objectlist.d ./Core/Src/objectlist.o ./Core/Src/objectlist.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

