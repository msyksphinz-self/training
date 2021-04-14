module tb_top;
`include "uvm_macros.svh"
import uvm_pkg::*;
`include "test.sv"
initial begin
  run_test();
end
endmodule
