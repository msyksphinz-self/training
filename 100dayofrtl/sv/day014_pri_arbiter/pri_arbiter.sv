module pri_arbiter
  #(parameter NUM_PORTS = 4)
(
 input logic [NUM_PORTS-1: 0] req_i,
 output logic [NUM_PORTS-1: 0] gnt_o
 );


assign gnt_o[0] = req_i[0];

generate for (genvar i = 1; i < NUM_PORTS; i++) begin : ports_loop
  assign gnt_o[i] = req_i[i] & ~(|req_i[i-1: 0]);
end
endgenerate

endmodule // pri_arbiter
