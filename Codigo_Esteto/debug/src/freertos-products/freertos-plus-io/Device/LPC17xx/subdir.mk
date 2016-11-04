################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Proyecto_Esteto/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_DriverInterface.c \
D:/Proyecto_Esteto/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_i2c.c \
D:/Proyecto_Esteto/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_ssp.c \
D:/Proyecto_Esteto/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_uart.c 

OBJS += \
./src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_DriverInterface.o \
./src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_i2c.o \
./src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_ssp.o \
./src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_uart.o 

C_DEPS += \
./src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_DriverInterface.d \
./src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_i2c.d \
./src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_ssp.d \
./src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_DriverInterface.o: D:/Proyecto_Esteto/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_DriverInterface.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\include" -I"D:\Proyecto_Esteto\CMSISv2p00_LPC17xx\inc" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\Examples\Include" -I"D:\Proyecto_Esteto\Esteto_v1.0\src" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\display" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\drivers" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\filtros" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\freeRTOS" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\adc-dac" -I"D:\Proyecto_Esteto\lpc17xx.cmsis.driver.library\Include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_i2c.o: D:/Proyecto_Esteto/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\include" -I"D:\Proyecto_Esteto\CMSISv2p00_LPC17xx\inc" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\Examples\Include" -I"D:\Proyecto_Esteto\Esteto_v1.0\src" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\display" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\drivers" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\filtros" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\freeRTOS" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\adc-dac" -I"D:\Proyecto_Esteto\lpc17xx.cmsis.driver.library\Include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_ssp.o: D:/Proyecto_Esteto/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_ssp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\include" -I"D:\Proyecto_Esteto\CMSISv2p00_LPC17xx\inc" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\Examples\Include" -I"D:\Proyecto_Esteto\Esteto_v1.0\src" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\display" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\drivers" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\filtros" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\freeRTOS" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\adc-dac" -I"D:\Proyecto_Esteto\lpc17xx.cmsis.driver.library\Include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_uart.o: D:/Proyecto_Esteto/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/FreeRTOS_lpc17xx_uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\Proyecto_Esteto\FreeRTOS-Products\FreeRTOS\include" -I"D:\Proyecto_Esteto\CMSISv2p00_LPC17xx\inc" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\Examples\Include" -I"D:\Proyecto_Esteto\Esteto_v1.0\src" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\display" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\drivers" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\filtros" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\freeRTOS" -I"D:\Proyecto_Esteto\Esteto_v1.0\src\adc-dac" -I"D:\Proyecto_Esteto\lpc17xx.cmsis.driver.library\Include" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


