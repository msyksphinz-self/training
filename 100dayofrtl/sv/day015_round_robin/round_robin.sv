module round_robin
  (
   input logic clk,
   input logic reset,
   input logic [ 3: 0] req_i,
   output logic [ 3: 0] gnt_o
   );


logic [ 3: 0]           mask_q;
logic [ 3: 0]           mask_next;

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    mask_q <= 4'b1111;
  end else begin
    mask_q <= mask_next;
  end
end

always_comb begin
  mask_next = mask_q;
  if      (gnt_o[0]) begin mask_next = 4'b1110; end
  else if (gnt_o[1]) begin mask_next = 4'b1100; end
  else if (gnt_o[2]) begin mask_next = 4'b1000; end
  else if (gnt_o[3]) begin mask_next = 4'b0000; end
end

logic [ 3: 0] mask_req;
assign mask_req = req_i & mask_q;

logic [ 3: 0] mask_gnt;
logic [ 3: 0] raw_gnt;

pri_arbiter #(4) u_mask_gnt (.req_i(mask_req), .gnt_o(mask_gnt));
pri_arbiter #(4) u_raw_gnt  (.req_i(req_i   ), .gnt_o(raw_gnt));

assign gnt_o = |mask_req ? mask_gnt : raw_gnt;

endmodule // round_robin
