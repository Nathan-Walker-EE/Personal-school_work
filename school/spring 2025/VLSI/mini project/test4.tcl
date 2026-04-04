read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
read_netlist HFC6.v
run_build_model HFC6
run_drc

set_patterns -delete
set_patterns -external test.stil
run_simulation -failure_file failure_HFC6.txt
run_diagnosis failure_HFC6.txt > diagnosis_HFC6.txt
