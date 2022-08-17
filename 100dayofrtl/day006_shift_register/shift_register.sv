module shift_register
  (
   input logic          clk,
   input logic          reset,

   input logic          x_i,

   output logic [ 3: 0] sr_o
   );

logic [ 3: 0]           sr_ff;
logic [ 3: 0]           sr_next;

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    sr_ff <= 4'h0;
  end else begin
    sr_ff <= sr_next;
  end
end

assign sr_next = {sr_ff[ 2: 0], x_i};

assign sr_o = sr_ff;

endmodule // shift_register
