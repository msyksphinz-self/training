.PHONY: all

FILELIST =
FILELIST += tb_top.sv
FILELIST += model/sample_if.sv

all:
	xvlog -sv $(FILELIST) -L uvm --include ./model/
	xelab tb_top -timescale 1ns/100ps -L uvm -debug all
	DUMP_TYPE=wdb xsim tb_top -R -t sim_vivado.tcl --testplusarg "UVM_TESTNAME=sample_test" -wdb out.wdb

clean:
	rm -rf *.log *.jou *.dir *.pb *.wdb
