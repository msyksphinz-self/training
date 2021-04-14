.PHONY: all

all:
	xvlog -sv tb_top.sv -L uvm
	xelab tb_top -timescale 1ns/100ps -L uvm
	xsim tb_top -R --testplusarg "UVM_TESNAME=test"

clean:
	rm -rf *.log *.jou *.dir *.pb
