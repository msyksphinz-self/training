module splitter
  #(parameter PORTS_N = 4)
(
 input logic                 i_clk,
 input logic                 i_reset_n,

 input logic                 i_master_valid,
 output logic                o_master_ready,

 output logic                o_sel_valid,
 input logic                 i_sel_ready,

 output logic [PORTS_N-1: 0] o_slave_valid,
 input  logic [PORTS_N-1: 0] i_slave_ready
 );

localparam PORTS_W = $clog2(PORTS_N);

logic                        w_fire;
logic [PORTS_W-1: 0]         r_sel;

always_ff @(posedge i_clk) begin
  if (!i_reset_n) begin
    r_sel <= 'h0;
  end else begin
    if (w_fire) begin
      r_sel <= r_sel + 'h1;
    end
  end
end

assign w_fire = i_master_valid & o_master_ready;

assign o_master_ready = i_slave_ready[r_sel] & i_sel_ready;
assign o_sel_valid = w_fire;

generate for (genvar idx = 0; idx < PORTS_N; idx++) begin : o_slave_loop
  assign o_slave_valid[idx] = w_fire & (r_sel == idx);
end
endgenerate

endmodule // splitter
