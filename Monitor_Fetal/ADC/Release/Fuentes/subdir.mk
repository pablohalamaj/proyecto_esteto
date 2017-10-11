################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Fuentes/Comunicaciones.c \
../Fuentes/Funciones.c \
../Fuentes/Main.c \
../Fuentes/Menu.c \
../Fuentes/Subfunciones.c \
../Fuentes/Touch.c \
../Fuentes/WG12864A.c \
../Fuentes/cr_startup_lpc11.c 

OBJS += \
./Fuentes/Comunicaciones.o \
./Fuentes/Funciones.o \
./Fuentes/Main.o \
./Fuentes/Menu.o \
./Fuentes/Subfunciones.o \
./Fuentes/Touch.o \
./Fuentes/WG12864A.o \
./Fuentes/cr_startup_lpc11.o 

C_DEPS += \
./Fuentes/Comunicaciones.d \
./Fuentes/Funciones.d \
./Fuentes/Main.d \
./Fuentes/Menu.d \
./Fuentes/Subfunciones.d \
./Fuentes/Touch.d \
./Fuentes/WG12864A.d \
./Fuentes/cr_startup_lpc11.d 


# Each subdirectory must supply rules for building sources it contributes
Fuentes/%.o: ../Fuentes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv1p30_LPC11xx -I"D:\Proyectos NXP6\CMSISv1p30_LPC11xx\inc" -O2 -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


