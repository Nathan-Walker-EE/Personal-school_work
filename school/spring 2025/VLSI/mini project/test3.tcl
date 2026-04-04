read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
read_netlist HFC4.v
run_build_model HFC4
run_drc

set_patterns -delete
set_patterns -external test.stil
run_simulation -failure_file failure_HFC4.txt
