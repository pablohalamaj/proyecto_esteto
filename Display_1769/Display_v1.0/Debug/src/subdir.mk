################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Funciones.c \
../src/Menu.c \
../src/Subfunciones.c \
../src/Touch.c \
../src/WG12864A.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/main.c 

OBJS += \
./src/Funciones.o \
./src/Menu.o \
./src/Subfunciones.o \
./src/Touch.o \
./src/WG12864A.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/main.o 

C_DEPS += \
./src/Funciones.d \
./src/Menu.d \
./src/Subfunciones.d \
./src/Touch.d \
./src/WG12864A.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__LPC17XX__ -I"F:\Proyectos LPC1769\Display_1769\CMSISv2p00_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/cr_startup_lpc175x_6x.o: ../src/cr_startup_lpc175x_6x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__LPC17XX__ -I"F:\Proyectos LPC1769\Display_1769\CMSISv2p00_LPC17xx\inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc175x_6x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


