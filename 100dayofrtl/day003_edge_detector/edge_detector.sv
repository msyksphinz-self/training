module edge_detector
  (
   input logic clk,
   input logic reset,

   input logic a_i,

   output logic rising_edge_o,
   output logic falling_edge_o
   );


logic           a_ff;

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    a_ff <= 1'b0;
  end else begin
    a_ff <= a_i;
  end
end

assign rising_edge_o  = ~a_ff &  a_i;
assign falling_edge_o =  a_ff & ~a_i;

endmodule; // edge_detector
