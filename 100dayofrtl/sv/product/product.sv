module product
  #(parameter W = 32)
(
 input logic           i_clk,
 input logic           i_reset_n,

 input logic           i_in_valid,
 output logic          o_in_ready,

 input logic [W-1: 0]  i_data0,
 input logic [W-1: 0]  i_data1,
 input logic [W-1: 0]  i_data2,
 input logic [W-1: 0]  i_data3,

 output logic          o_out_valid,
 output logic [W-1: 0] o_out_data,
 input logic           i_out_ready
 );

typedef enum logic [ 1: 0] {
  S1 = 2'b01,
  S2 = 2'b10,
  S3 = 2'b11
} state_t;

state_t r_state;
state_t w_state_next;

logic [W-1: 0] o_out_next;

logic        w_stall;
assign w_stall = o_out_valid & !i_out_ready;

logic [W-1: 0] w_y_out;
logic [W-1: 0] aIn;
logic [W-1: 0] bIn;
assign w_y_out = aIn * bIn;

always_comb begin
  if (!w_stall) begin
    case (r_state)
      S1 : if (i_in_valid) begin
        w_state_next = S2;
        aIn = i_data0;
        bIn = i_data1;
        o_out_next = w_y_out;
      end
      S2 : begin
        w_state_next = S3;
        aIn = o_out_data;
        bIn = i_data2;
        o_out_next = w_y_out;
      end
      S3 : begin
        w_state_next = S1;
        aIn = o_out_data;
        bIn = i_data3;
        o_out_next = w_y_out;
      end
      default : begin
        w_state_next = S1;
        aIn = 'h0;
        bIn = 'h0;
        o_out_next = 'h0;
    end // if (!w_stall)
    endcase // case (r_state)
  end else begin
    w_state_next = r_state;
    aIn = 'h0;
    bIn = 'h0;
    o_out_next = 'h0;
  end // if (!w_stall)

end // always_comb

always_ff @ (posedge i_clk, negedge i_reset_n) begin
  if (!i_reset_n) begin
    r_state <= S1;
  end else begin
    r_state <= w_state_next;
  end
end

always_ff @ (posedge i_clk) begin
  o_out_data <= o_out_next;
  o_out_valid <= (r_state == S3) & !w_stall;
end

assign o_in_ready = (r_state != S1);

endmodule // product
