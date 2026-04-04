# Load sources
read_verilog [ glob src/*.v ]
read_xdc unsigned_arithmetic.xdc

# Run Synthesis

synth_design -top up_down_counter -part xc7a35tcpg236-1
write_verilog -force post_synth.v

# Implement (optimize, place, route)
opt_design
place_design
route_design


# Generate Reports
report_timing_summary -file post_route_timing.rpt
report_utilization -file post_route_utilization.rpt

# Make bitstream
write_bitstream -force unsigned_arithmetic.bit

