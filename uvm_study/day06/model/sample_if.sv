interface sample_if(input logic clk, rstz);
  logic write;   // 1: write, 0: read
  logic valid;
  logic ready;
  logic [7: 0] addr, wdata, rdata;
endinterface // sample_if
