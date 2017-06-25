ghdl -a --workdir=simu src/*.vhd
ghdl -a --workdir=simu benches/*.vhd

ghdl -e --workdir=simu myflasher_tb
ghdl -r myflasher_rb --vcd=simu/myflasher.vcd --stop-time=100ms
gtkwave simu/myflasher.vcd
