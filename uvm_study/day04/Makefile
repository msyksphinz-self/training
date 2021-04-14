.PHONY: all

FILELIST =
FILELIST += tb_top.sv

all:
	xvlog -sv $(FILELIST) -L uvm --include ./model/
	xelab tb_top -timescale 1ns/100ps -L uvm
	xsim tb_top -R --testplusarg "UVM_TESTNAME=sample_test"


clean:
	rm -rf *.log *.jou *.dir *.pb
