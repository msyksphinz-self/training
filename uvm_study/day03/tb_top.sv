module tb_top;
// UVM class library
`include "uvm_macros.svh"
import uvm_pkg::*;
// uvm user code
`include "sample_model.svh"
`include "sample_test.sv"
//////////////////////////////////////
initial begin
  run_test();
end

endmodule // tb_top
