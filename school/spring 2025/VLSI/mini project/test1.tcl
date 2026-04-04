read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
read_netlist Circuit.v
run_build_model Circuit
run_drc
remove_faults -all
add_faults -all

set_atpg -coverage 90
run_atpg -optimize

write_patterns test.stil -format stil
run_simulation
report_faults -summary > fault.txt
write_faults Good.txt -all -replace
