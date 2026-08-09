################################################################################
# Clock
################################################################################

set_property -dict { PACKAGE_PIN L17 IOSTANDARD LVCMOS33 } [get_ports clk]
create_clock -period 83.33 -name sys_clk_pin [get_ports clk]

################################################################################
# 16-bit Bidirectional Data Bus
################################################################################

set_property -dict { PACKAGE_PIN M3 IOSTANDARD LVCMOS33 } [get_ports {data[0]}]
set_property -dict { PACKAGE_PIN L3 IOSTANDARD LVCMOS33 } [get_ports {data[1]}]
set_property -dict { PACKAGE_PIN A16 IOSTANDARD LVCMOS33 } [get_ports {data[2]}]
set_property -dict { PACKAGE_PIN K3 IOSTANDARD LVCMOS33 } [get_ports {data[3]}]
set_property -dict { PACKAGE_PIN C15 IOSTANDARD LVCMOS33 } [get_ports {data[4]}]
set_property -dict { PACKAGE_PIN H1 IOSTANDARD LVCMOS33 } [get_ports {data[5]}]
set_property -dict { PACKAGE_PIN A15 IOSTANDARD LVCMOS33 } [get_ports {data[6]}]
set_property -dict { PACKAGE_PIN B15 IOSTANDARD LVCMOS33 } [get_ports {data[7]}]
set_property -dict { PACKAGE_PIN A14 IOSTANDARD LVCMOS33 } [get_ports {data[8]}]
set_property -dict { PACKAGE_PIN J3 IOSTANDARD LVCMOS33 } [get_ports {data[9]}]
set_property -dict { PACKAGE_PIN J1 IOSTANDARD LVCMOS33 } [get_ports {data[10]}]
set_property -dict { PACKAGE_PIN K2 IOSTANDARD LVCMOS33 } [get_ports {data[11]}]
set_property -dict { PACKAGE_PIN L1 IOSTANDARD LVCMOS33 } [get_ports {data[12]}]
set_property -dict { PACKAGE_PIN L2 IOSTANDARD LVCMOS33 } [get_ports {data[13]}]
set_property -dict { PACKAGE_PIN M1 IOSTANDARD LVCMOS33 } [get_ports {data[14]}]
set_property -dict { PACKAGE_PIN N3 IOSTANDARD LVCMOS33 } [get_ports {data[15]}]

################################################################################
# Control Inputs
################################################################################

set_property -dict { PACKAGE_PIN P3 IOSTANDARD LVCMOS33 } [get_ports write_enable_async]
set_property -dict { PACKAGE_PIN M2 IOSTANDARD LVCMOS33 } [get_ports read_enable_async]
set_property -dict { PACKAGE_PIN N1 IOSTANDARD LVCMOS33 } [get_ports rst]
set_property -dict { PACKAGE_PIN N2 IOSTANDARD LVCMOS33 } [get_ports execute_async]

################################################################################
# Operation[4:0]
################################################################################

set_property -dict { PACKAGE_PIN P1 IOSTANDARD LVCMOS33 } [get_ports {operation[0]}]
set_property -dict { PACKAGE_PIN R3 IOSTANDARD LVCMOS33 } [get_ports {operation[1]}]
set_property -dict { PACKAGE_PIN T3 IOSTANDARD LVCMOS33 } [get_ports {operation[2]}]
set_property -dict { PACKAGE_PIN R2 IOSTANDARD LVCMOS33 } [get_ports {operation[3]}]
set_property -dict { PACKAGE_PIN T1 IOSTANDARD LVCMOS33 } [get_ports {operation[4]}]

################################################################################
# Address
################################################################################

set_property -dict { PACKAGE_PIN T2 IOSTANDARD LVCMOS33 } [get_ports address_enable_async]

set_property -dict { PACKAGE_PIN U1 IOSTANDARD LVCMOS33 } [get_ports {address[0]}]
set_property -dict { PACKAGE_PIN W2 IOSTANDARD LVCMOS33 } [get_ports {address[1]}]
set_property -dict { PACKAGE_PIN V2 IOSTANDARD LVCMOS33 } [get_ports {address[2]}]
set_property -dict { PACKAGE_PIN W3 IOSTANDARD LVCMOS33 } [get_ports {address[3]}]
set_property -dict { PACKAGE_PIN V3 IOSTANDARD LVCMOS33 } [get_ports {address[4]}]

################################################################################
# Status Outputs
################################################################################

set_property -dict { PACKAGE_PIN W5 IOSTANDARD LVCMOS33 } [get_ports done]
set_property -dict { PACKAGE_PIN V4 IOSTANDARD LVCMOS33 } [get_ports error]
set_property -dict { PACKAGE_PIN U4 IOSTANDARD LVCMOS33 } [get_ports ack]
