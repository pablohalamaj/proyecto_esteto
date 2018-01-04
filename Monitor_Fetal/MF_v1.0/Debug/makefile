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
all: MF_v1.0.axf

# Tool invocations
MF_v1.0.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"F:\Proyectos LPC1115\Monitor_Fetal\CMSISv1p30_LPC11xx\Debug" -Xlinker -Map="MF_v1.0.map" -Xlinker --gc-sections -mcpu=cortex-m0 -mthumb -T "Touch_2_2_Debug.ld" -o "MF_v1.0.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) MF_v1.0.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MF_v1.0.axf"; # arm-none-eabi-objcopy -O binary "MF_v1.0.axf" "MF_v1.0.bin" ; checksum -p LPC1114_302 -d "MF_v1.0.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
