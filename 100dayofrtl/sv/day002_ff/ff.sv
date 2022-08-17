module ff
  (
   input logic  clk,
   input logic  reset,

   input logic  d_i,

   output logic q_norst_o,
   output logic q_syncrst_o,
   output logic q_asyncrst_o
   );

always_ff @ (posedge clk) begin
  q_norst_o <= d_i;
end

always_ff @ (posedge clk) begin
  if (reset) begin
    q_syncrst_o <= 1'b0;
  end else begin
    q_syncrst_o <= d_i;
  end
end

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    q_asyncrst_o <= 1'b0;
  end else begin
    q_asyncrst_o <= d_i;
  end
end

endmodule // ff
