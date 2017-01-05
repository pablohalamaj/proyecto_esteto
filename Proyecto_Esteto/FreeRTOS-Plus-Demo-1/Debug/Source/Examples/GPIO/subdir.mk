################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/Examples/GPIO/GPIO-output-and-software-timers.c 

OBJS += \
./Source/Examples/GPIO/GPIO-output-and-software-timers.o 

C_DEPS += \
./Source/Examples/GPIO/GPIO-output-and-software-timers.d 


# Each subdirectory must supply rules for building sources it contributes
Source/Examples/GPIO/%.o: ../Source/Examples/GPIO/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\include" -I"D:\Proyecto_Esteto\CMSISv2p00_LPC17xx\inc" -I"D:\Proyecto_Esteto\FreeRTOS-Plus-Demo-1\Source\Examples\Include" -I"D:\Proyecto_Esteto\FreeRTOS-Plus-Demo-1\Source" -I"D:\Proyecto_Esteto\FreeRTOS-Plus-Demo-1\Source\adc-dac" -I"D:\Proyecto_Esteto\FreeRTOS-Plus-Demo-1\Source\display" -I"D:\Proyecto_Esteto\FreeRTOS-Plus-Demo-1\Source\filtros" -I"D:\Proyecto_Esteto\lpc17xx.cmsis.driver.library\Include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


