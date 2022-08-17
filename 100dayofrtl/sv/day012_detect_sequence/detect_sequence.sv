module detect_squence
  (
   input logic clk,
   input logic reset,
   input logic x_i,

   output logic detect_o
   );


logic [11: 0]   shift_ff;
logic [11: 0]   shift_next;

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    shift_ff <= 'h0;
  end else begin
    shift_ff <= shift_next;
  end
end

assign shift_next = {shift_ff[10: 0], x_i};

assign detect_o = (shift_ff == 12'b1110_1101_1011);

endmodule // detect_squence
