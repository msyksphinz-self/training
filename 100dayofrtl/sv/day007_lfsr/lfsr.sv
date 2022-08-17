module lfsr
  (
   input logic clk,
   input logic reset,

   output logic [ 3: 0] lfsr_o
   );


logic [ 3: 0]           lfsr_ff;
logic [ 3: 0]           lfsr_next;

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    lfsr_ff <= 'he;
  end else begin
    lfsr_ff <= lfsr_next;
  end
end

assign lfsr_next = {lfsr_ff[ 2: 0], lfsr_ff[1] ^ lfsr_ff[3]};

assign lfsr_o = lfsr_ff;

endmodule // lfsr
