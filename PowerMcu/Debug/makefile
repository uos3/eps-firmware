################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./core.obj" \
"./main.obj" \
"./hal/adc.obj" \
"./hal/hal.obj" \
"./hal/i2c.obj" \
"./hal/intpin.obj" \
"./hal/mux.obj" \
"./hal/stopwatch.obj" \
"./hal/stopwatchB.obj" \
"./hal/switches.obj" \
"./hal/uart.obj" \
"./hal/watchdog.obj" \
"./peripheral/RailOffCheck.obj" \
"./peripheral/battery.obj" \
"./peripheral/comm.obj" \
"./peripheral/deploy.obj" \
"./peripheral/errormon.obj" \
"./peripheral/interrupt.obj" \
"./peripheral/ocp.obj" \
"./peripheral/register.obj" \
"./peripheral/sense.obj" \
"./peripheral/status.obj" \
"./util/DCO_Library.obj" \
"./util/circ.obj" \
"./util/crc.obj" \
"../lnk_msp430f2132.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include hal/subdir_vars.mk
-include peripheral/subdir_vars.mk
-include util/subdir_vars.mk
-include subdir_rules.mk
-include hal/subdir_rules.mk
-include peripheral/subdir_rules.mk
-include util/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
PowerMcu\ Working4.out \

EXE_OUTPUTS__QUOTED += \
"PowerMcu Working4.out" \

BIN_OUTPUTS += \
PowerMcu\ Working4.hex \

BIN_OUTPUTS__QUOTED += \
"PowerMcu Working4.hex" \


# All Target
all: PowerMcu\ Working4.out

# Tool invocations
PowerMcu\ Working4.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/bin/cl430" -vmsp --abi=eabi -Ooff --opt_for_speed=1 --use_hw_mpy=none --advice:power="all" -g --define=__MSP430F2132__ --display_error_number --diag_warning=225 --diag_wrap=off --printf_support=minimal -z -m"PowerMcu Working4.map" --stack_size=80 --heap_size=80 -i"C:/ti/ccs910/ccs/ccs_base/msp430/include" -i"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/lib" -i"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/include" --reread_libs --warn_sections --diag_wrap=off --display_error_number --xml_link_info="PowerMcu Working4_linkInfo.xml" --use_hw_mpy=none --rom_model -o "PowerMcu Working4.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

PowerMcu\ Working4.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: MSP430 Hex Utility'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/bin/hex430" --memwidth=8 --romwidth=8 -o "PowerMcu Working4.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "core.obj" "main.obj" "hal\adc.obj" "hal\hal.obj" "hal\i2c.obj" "hal\intpin.obj" "hal\mux.obj" "hal\stopwatch.obj" "hal\stopwatchB.obj" "hal\switches.obj" "hal\uart.obj" "hal\watchdog.obj" "peripheral\RailOffCheck.obj" "peripheral\battery.obj" "peripheral\comm.obj" "peripheral\deploy.obj" "peripheral\errormon.obj" "peripheral\interrupt.obj" "peripheral\ocp.obj" "peripheral\register.obj" "peripheral\sense.obj" "peripheral\status.obj" "util\DCO_Library.obj" "util\circ.obj" "util\crc.obj" 
	-$(RM) "core.d" "main.d" "hal\adc.d" "hal\hal.d" "hal\i2c.d" "hal\intpin.d" "hal\mux.d" "hal\stopwatch.d" "hal\stopwatchB.d" "hal\switches.d" "hal\uart.d" "hal\watchdog.d" "peripheral\RailOffCheck.d" "peripheral\battery.d" "peripheral\comm.d" "peripheral\deploy.d" "peripheral\errormon.d" "peripheral\interrupt.d" "peripheral\ocp.d" "peripheral\register.d" "peripheral\sense.d" "peripheral\status.d" "util\circ.d" "util\crc.d" 
	-$(RM) "util\DCO_Library.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

