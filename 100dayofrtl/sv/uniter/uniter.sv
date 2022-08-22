module uniter
  #(
    parameter PORTS_N = 4,
    parameter PORTS_W = 1 << PORTS_N
    )
(
 input logic                 i_clk,
 input logic                 i_reset_n,

 input logic [PORTS_W-1: 0]  i_master_valid,
 output logic [PORTS_W-1: 0] o_master_ready,

 input logic [PORTS_N-1: 0]  i_sel,

 output logic                o_slave_valid,
 input logic                 i_slave_ready
 );

assign w_fire = i_master_valid & i_slave_ready;

assign o_slave_valid = i_master_valid[i_sel];
generate for (genvar idx = 0; idx < PORTS_N; idx++) begin : master_loop
  assign o_master_ready[idx] = (i_sel == idx) & i_slave_ready;
end
endgenerate


endmodule // uniter
