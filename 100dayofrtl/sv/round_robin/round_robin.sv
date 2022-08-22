module round_robin
  #(parameter PORTS_N = 3,
    localparam PORTS_W = $clog2(PORTS_N))
(
 input logic i_clk,
 input logic i_reset_n,

 input logic [PORTS_N-1: 0] i_master_valid,
 input logic [PORTS_N-1: 0] o_master_ready,

 output logic o_slave_valid,
 input logic i_slave_ready,
 );

logic [PORTS_N-1: 0] r_ptr_oh;

logic                w_slave_fire;
assign w_slave_fire = o_slave_valid & i_slave_ready;

always_ff @ (posedge i_clk, negedge i_reset_n) begin
  if (!i_reset_n) begin
    r_ptr_oh <= {{(PORTS_N-1){1'b0}}, 1'b1};
  end else begin
    if (w_slave_fire) begin
      r_ptr_oh <= {r_ptr_oh[PORTS_N-2: 0], r_ptr_oh[PORTS_N-1]};
    end
  end
end

logic [PORTS_W-1: 0] num_idx[PORTS_N];
generate for (genvar i_idx = 0; i_idx < PORTS_N; i_idx++) begin
  always_comb begin
    num_idx [i_idx] = 'h0;
    for (genvar j_idx = 0; j_idx < PORTS_N; j_idx++) begin
      integer idx = i_idx[$clog2(PORTS_N)-1: 0]  + j_idx[$clog2(PORTS_N)-1: 0];
      if (i_master_valid[idx]) begin
        num_idx[i_idx] = idx;
      end
    end
  end
end
endgenerate

o_selected = num_idx[r_ptr_oh];

endmodule // round_robin
