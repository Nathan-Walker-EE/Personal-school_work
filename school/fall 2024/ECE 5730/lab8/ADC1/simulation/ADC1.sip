set_global_assignment -entity "ADC1" -library "lib_ADC1" -name IP_TOOL_NAME "Qsys"
set_global_assignment -entity "ADC1" -library "lib_ADC1" -name IP_TOOL_VERSION "23.1"
set_global_assignment -entity "ADC1" -library "lib_ADC1" -name IP_TOOL_ENV "Qsys"
set_global_assignment -library "lib_ADC1" -name SPD_FILE [file join $::quartus(sip_path) "../ADC1.spd"]
set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "../../ADC1.qsys"]

set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "ADC1.vhd"]
set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "submodules/ADC1_modular_adc_0.vhd"]
set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "submodules/altera_modular_adc_control.v"]
set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "submodules/altera_modular_adc_control_avrg_fifo.v"]
set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "submodules/altera_modular_adc_control_fsm.v"]
set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "submodules/chsel_code_converter_sw_to_hw.v"]
set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "submodules/fiftyfivenm_adcblock_primitive_wrapper.v"]
set_global_assignment -library "lib_ADC1" -name MISC_FILE [file join $::quartus(sip_path) "submodules/fiftyfivenm_adcblock_top_wrapper.v"]
