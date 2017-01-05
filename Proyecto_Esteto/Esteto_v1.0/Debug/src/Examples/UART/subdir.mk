################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Examples/UART/CLI-commands.c \
../src/Examples/UART/UART-interrupt-driven-command-console.c \
../src/Examples/UART/UART-operation-modes-test.c 

OBJS += \
./src/Examples/UART/CLI-commands.o \
./src/Examples/UART/UART-interrupt-driven-command-console.o \
./src/Examples/UART/UART-operation-modes-test.o 

C_DEPS += \
./src/Examples/UART/CLI-commands.d \
./src/Examples/UART/UART-interrupt-driven-command-console.d \
./src/Examples/UART/UART-operation-modes-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/Examples/UART/%.o: ../src/Examples/UART/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\include" -I"D:\Proyecto_Esteto\CMSISv2p00_LPC17xx\inc" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\Examples\Include" -I"D:\Proyecto_Esteto\Esteto_v1.0\src" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\display" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\drivers" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\filtros" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\freeRTOS" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\adc-dac" -I"D:\Proyecto_Esteto\lpc17xx.cmsis.driver.library\Include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


