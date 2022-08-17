module odd_counter
  (
   input logic clk,
   input logic reset,

   output logic [ 7: 0] cnt_o
   );


logic [ 7: 0]           cnt_next;

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    cnt_o <= 8'h1;
  end else begin
    cnt_o <= cnt_next;
  end
end

assign cnt_next = cnt_o + 8'h2;

endmodule // odd_counter
