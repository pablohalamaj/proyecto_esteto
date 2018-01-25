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
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv1p30_LPC11xx -D__REDLIB__ -I"F:\Proyectos LPC1115\Monitor_Fetal\CMSISv1p30_LPC11xx\inc" -I"F:\Proyectos LPC1115\Monitor_Fetal\MF_v1.1\Cabeceras" -I"F:\Proyectos LPC1115\Monitor_Fetal\MF_v1.1\Drivers" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


