################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
util/%.obj: ../util/%.asm $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/bin/cl430" -vmsp -Ooff --opt_for_speed=1 --use_hw_mpy=none --include_path="C:/ti/ccs910/ccs/ccs_base/msp430/include" --include_path="C:/ti/ccs910/ccs/ccs_base/msp430/include" --include_path="C:/Users/HP User/workspace_v9/PowerMcu Working4" --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/include" --advice:power="all" --define=__MSP430F2132__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU19 --preproc_with_compile --preproc_dependency="util/$(basename $(<F)).d_raw" --obj_directory="util" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

util/%.obj: ../util/%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/bin/cl430" -vmsp -Ooff --opt_for_speed=1 --use_hw_mpy=none --include_path="C:/ti/ccs910/ccs/ccs_base/msp430/include" --include_path="C:/ti/ccs910/ccs/ccs_base/msp430/include" --include_path="C:/Users/HP User/workspace_v9/PowerMcu Working4" --include_path="C:/ti/ccs910/ccs/tools/compiler/ti-cgt-msp430_18.12.2.LTS/include" --advice:power="all" --define=__MSP430F2132__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU19 --preproc_with_compile --preproc_dependency="util/$(basename $(<F)).d_raw" --obj_directory="util" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


