module bin_to_gray
  #(
    parameter VEC_W = 4
    )
(
 input logic[VEC_W-1: 0] bin_i,
 output logic [VEC_W-1: 0] gray_o
 );


assign gray_o[VEC_W-1] = bin_i[VEC_W-1];

generate for (genvar i = VEC_W-2; i >= 0; i--) begin : gray_loop
  assign gray_o[i] = bin_i[i+1] ^ bin_i[i];
end
endgenerate

endmodule // bin_to_gray
