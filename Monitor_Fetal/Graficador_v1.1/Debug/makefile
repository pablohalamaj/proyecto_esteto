################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Fuentes/subdir.mk
-include Drivers/subdir.mk
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
all: Graficador_v1.1.axf

# Tool invocations
Graficador_v1.1.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/sebastian/LPCXpresso/workspace/Proyectos LPC1115/Monitor_Fetal/CMSISv1p30_LPC11xx/Debug" -Xlinker -Map="Graficador_v1.1.map" -Xlinker --gc-sections -mcpu=cortex-m0 -mthumb -T "Touch_2_2_Debug.ld" -o "Graficador_v1.1.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Graficador_v1.1.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Graficador_v1.1.axf"; # arm-none-eabi-objcopy -O binary "Graficador_v1.1.axf" "Graficador_v1.1.bin" ; checksum -p LPC1115_303 -d "Graficador_v1.1.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
