################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/adc.c \
../Drivers/gpio.c \
../Drivers/ssp.c \
../Drivers/timer32.c \
../Drivers/wdt.c 

OBJS += \
./Drivers/adc.o \
./Drivers/gpio.o \
./Drivers/ssp.o \
./Drivers/timer32.o \
./Drivers/wdt.o 

C_DEPS += \
./Drivers/adc.d \
./Drivers/gpio.d \
./Drivers/ssp.d \
./Drivers/timer32.d \
./Drivers/wdt.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o: ../Drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv1p30_LPC11xx -I"D:\Proyectos NXP6\CMSISv1p30_LPC11xx\inc" -O2 -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


