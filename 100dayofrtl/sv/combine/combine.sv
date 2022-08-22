module combine
  #(parameter N = 4)
(
 input logic i_clk,
 input logic i_reset_n,

 input  logic [N-1: 0] i_master_valid,
 output logic [N-1: 0] o_master_ready,
 input  logic [N-1: 0] i_master_end,

 output logic o_slave_valid,
 input  logic i_slave_ready
 );

typedef enum logic [1:0]
{
 IDLE  = 2'b00,
 PASS  = 2'b10,
 BLOCK = 2'b11
} state_t;

state_t r_cnd[N];

logic [N-1: 0] w_alloc;
logic          w_done;
logic [N-1: 0] r_dummy_stall;
logic [N-1: 0] r_dummy_valid;

logic          w_nasc;
// w_nascの条件 : すべての入力が受け入れられている
// 次のリクエストが受け付けられる
assign w_nasc  = &(i_master_valid | r_dummy_valid) & !i_slave_ready;

assign w_alloc = i_master_valid & o_master_ready & i_master_end;
assign w_done  = &(w_alloc | r_dummy_stall);

generate for (genvar idx = 0; idx < N; idx++) begin : cnd_loop
  always_ff @ (posedge i_clk, negedge i_reset_n) begin
    if (!i_reset_n) begin
      r_cnd[idx] <= IDLE;
    end else begin
      r_cnd[idx] <= w_cnd_next;
    end
  end

  state_t w_cnd_next;
  logic  w_dummy_valid_next;
  logic  w_dummy_stall_next;

  always_comb begin
    w_dummy_valid_next = (r_cnd[idx] == PASS);
    w_dummy_stall_next = (r_cnd[idx] == BLOCK);
  end

  always_ff @ (posedge i_clk, negedge i_reset_n) begin
    if (!i_reset_n) begin
      r_dummy_valid[idx] <= 1'b0;
      r_dummy_stall[idx] <= 1'b0;
    end else begin
      r_dummy_valid[idx] <= w_dummy_valid_next;
      r_dummy_stall[idx] <= w_dummy_stall_next;
    end
  end

  always_comb begin
    w_cnd_next = r_cnd[idx];

    case (r_cnd[idx])
      IDLE : begin
        /* verilator lint_off CASEX */
        casex ({w_alloc[idx], w_alloc[0]})
          2'b01 : w_cnd_next = PASS;
          2'b1x : w_cnd_next = BLOCK;
          default : w_cnd_next = r_cnd[idx];
        endcase // casex ({w_done, w_alloc[idx], w_alloc[0]})
      end
      PASS : begin
        /* verilator lint_off CASEX */
        casex ({w_done, w_alloc[idx]})
          2'b1x : w_cnd_next = IDLE;
          2'b01 : w_cnd_next = BLOCK;
          default : w_cnd_next = r_cnd[idx];
        endcase // casex ({w_done, w_alloc[idx]})
      end
      BLOCK : begin
        if (w_done) begin
          w_cnd_next = IDLE;
        end
      end
      default : begin
        w_cnd_next = IDLE;
      end
    endcase // case (r_cnd[idx])
  end // always_comb

  assign o_master_ready[idx] = !r_dummy_stall[idx] & w_nasc;

end
endgenerate



endmodule // combine
