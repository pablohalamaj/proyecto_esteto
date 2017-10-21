################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Fuentes/Funciones.c \
../Fuentes/Main.c \
../Fuentes/Teclado.c \
../Fuentes/WG12864A.c \
../Fuentes/cr_startup_lpc11.c 

OBJS += \
./Fuentes/Funciones.o \
./Fuentes/Main.o \
./Fuentes/Teclado.o \
./Fuentes/WG12864A.o \
./Fuentes/cr_startup_lpc11.o 

C_DEPS += \
./Fuentes/Funciones.d \
./Fuentes/Main.d \
./Fuentes/Teclado.d \
./Fuentes/WG12864A.d \
./Fuentes/cr_startup_lpc11.d 


# Each subdirectory must supply rules for building sources it contributes
Fuentes/%.o: ../Fuentes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv1p30_LPC11xx -D__REDLIB__ -I"/home/sebastian/LPCXpresso/workspace/Proyectos LPC1115/Monitor_Fetal/CMSISv1p30_LPC11xx/inc" -I"/home/sebastian/LPCXpresso/workspace/Proyectos LPC1115/Monitor_Fetal/Graficador_v1.1/Cabeceras" -I"/home/sebastian/LPCXpresso/workspace/Proyectos LPC1115/Monitor_Fetal/Graficador_v1.1/Drivers" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Fuentes/cr_startup_lpc11.o: ../Fuentes/cr_startup_lpc11.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv1p30_LPC11xx -D__REDLIB__ -I"/home/sebastian/LPCXpresso/workspace/Proyectos LPC1115/Monitor_Fetal/CMSISv1p30_LPC11xx/inc" -I"/home/sebastian/LPCXpresso/workspace/Proyectos LPC1115/Monitor_Fetal/Graficador_v1.1/Cabeceras" -I"/home/sebastian/LPCXpresso/workspace/Proyectos LPC1115/Monitor_Fetal/Graficador_v1.1/Drivers" -Os -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"Fuentes/cr_startup_lpc11.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


