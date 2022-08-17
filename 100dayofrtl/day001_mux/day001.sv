module mux
  (
   input logic [ 7: 0]  a_i,
   input logic [ 7: 0]  b_i,
   input logic          sel_i,
   output logic [ 7: 0] y_o
   );

assign y_o = sel_i ? a_i : b_i;

endmodule // mux
