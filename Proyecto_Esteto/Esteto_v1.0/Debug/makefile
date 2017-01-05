################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/filtros/subdir.mk
-include src/display/subdir.mk
-include src/adc-dac/subdir.mk
-include src/FreeRTOS-Products/FreeRTOS/portable/MemMang/subdir.mk
-include src/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3/subdir.mk
-include src/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/subdir.mk
-include src/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/subdir.mk
-include src/FreeRTOS-Products/FreeRTOS-Plus-CLI/subdir.mk
-include src/FreeRTOS-Products/FreeRTOS/subdir.mk
-include src/Examples/UART/subdir.mk
-include src/Examples/SSP/subdir.mk
-include src/Examples/I2C/subdir.mk
-include src/Examples/GPIO/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Esteto_v1.0.axf

# Tool invocations
Esteto_v1.0.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\Proyecto_Esteto\lpc17xx.cmsis.driver.library\Debug" -L"D:\Proyecto_Esteto\CMSISv2p00_LPC17xx\Debug" -Xlinker -Map=Esteto_v1.0.map -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "Esteto_v1_0_Debug.ld" -o "Esteto_v1.0.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Esteto_v1.0.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size Esteto_v1.0.axf; # arm-none-eabi-objcopy -O binary Esteto_v1.0.axf Esteto_v1.0.bin ; checksum -p LPC1769 -d Esteto_v1.0.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
