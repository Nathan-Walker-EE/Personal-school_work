#regular circuit
read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
read_netlist /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/lab5_circuit-3.v
run_build_model circuit_3
run_drc
remove_faults -all
add_faults -all
reset_state

# # buffer circuit
#read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
#read_netlist /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/lab5_circuit-1buff.v
#run_build_model circuit_1
#run_drc
#remove_faults -all
#this is n2
# # add_faults U11/ZN -stuck 0 
#add_faults PO2 -stuck 1
#run_atpg basic_scan_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/basic_scan-1.stil -format stil -replace
#run_simulation
#report_faults -all
#report_patterns -verbose

# #3 modes atpg
#read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
#read_netlist /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/lab5_circuit-1.v
#run_build_model circuit_1
#set_atpg -capture_cycles 3
#run_drc
#remove_faults -all
# # this is n2
#add_faults U11/ZN -stuck 0 
#add_faults PO2 -stuck 1
#run_atpg basic_scan_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/basic_scan-original.stil -format stil -replace
#run_atpg fast_sequential_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/fast_scan-original.stil -format stil -replace
#run_atpg full_sequential_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/full_scan-original.stil -format stil -replace
#run_simulation
#report_faults -all
#report_patterns -verbose

# #collapsed circuits for each fault list
#read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
#read_netlist /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/lab5_circuit-1.v
#run_build_model circuit_1
# set_atpg -capture_cycles 3
# run_drc
# remove_faults -all
# #you can use the collapsed circuits to add the faults here
#read_faults /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/Collapsed_Circuit_1 
# # this is n2
#add_faults U11/ZN -stuck 0 
#add_faults PO2 -stuck 1
#run_atpg basic_scan_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/basic_scan-collapsed.stil -format stil -replace
#run_atpg fast_sequential_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/fast_scan-collapsed.stil -format stil -replace
#run_atpg full_sequential_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/full_scan-collapsed.stil -format stil -replace
#run_simulation
#report_faults -all

#circuit 2 collapsed
#read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
#read_netlist /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/lab5_circuit-2.v
#run_build_model circuit_2
#add_clocks 0 clk
# set_atpg -capture_cycles 3
# run_drc
# remove_faults -all
#add_faults -all
# write_faults /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/Collapsed_Circuit_2 -all -replace -collapsed
#remove_faults -all
# # #you can use the collapsed circuits to add the faults here
#read_faults /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/Collapsed_Circuit_2
#run_atpg basic_scan_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/basic_scan-collapsed_Circuit_2.stil -format stil -replace
#run_atpg fast_sequential_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/fast_scan-collapsed_Circuit_2.stil -format stil -replace
#run_atpg full_sequential_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/full_scan-collapsed_Circuit_2.stil -format stil -replace
#run_simulation
#report_faults -all

#circuit 3 collapsed
#read_netlist /opt/cadence_roy/library/tcbn45nm/verilog/HVT/tcbn45gsbwphvt.v -library
#read_netlist /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/lab5_circuit-3.v
#run_build_model circuit_3
#add_clocks 0 clk
# set_atpg -capture_cycles 3
# run_drc
# remove_faults -all
#add_faults -all
# write_faults /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/Collapsed_Circuit_3 -all -replace -collapsed
#remove_faults -all
# #you can use the collapsed circuits to add the faults here
#read_faults /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/Collapsed_Circuit_3
#run_atpg basic_scan_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/basic_scan-collapsed_Circuit_3.stil -format stil -replace
#run_atpg fast_sequential_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/fast_scan-collapsed_Circuit_3.stil -format stil -replace
#run_atpg full_sequential_only
#write_patterns /mnt/homes/a02370680/Desktop/VLSI/5480_projects/Lab5/full_scan-collapsed_Circuit_3.stil -format stil -replace
#run_simulation
#report_faults -all

# # this grabs collapsed faults
# #You can add clocks like this and run code like so
#add_clocks 0 clk
#run_drc
#remove_faults -all
#add_faults -all
#write_faults Collapsed_circuit_2 -all -replace -collapsed
#remove_faults -all
#read_faults Collapsed_circuit_2
#run_atpg basic_scan_only
#report_patterns -all