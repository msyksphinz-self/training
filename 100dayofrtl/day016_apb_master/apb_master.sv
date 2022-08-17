// APB Master

// TB should drive a cmd_i input decoded as:
//  - 2'b00 - No-op
//  - 2'b01 - Read from address 0xDEAD_CAFE
//  - 2'b10 - Increment the previously read data and store it to 0xDEAD_CAFE

module apb_master
  (
   input logic          clk,
   input logic          reset,
   input logic [ 1: 0]  cmd_i,

   output logic         psel_o,
   output logic         penable_o,
   output logic [31: 0] paddr_o,
   output logic         pwrite_o,
   output logic [31: 0] pwdata_o,

   input logic          pready_i,
   output logic [31: 0] prdata_i
   );


typedef enum logic [ 1: 0]
{
 ST_IDLE = 2'b00,
 ST_SETUP = 2'b01,
 ST_ACCESS = 2'b10
 } apb_state_t;

apb_state_t state_next;
apb_state_t state_q;

logic [31: 0] rdata_q;


always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    state_q <= ST_IDLE;
  end else begin
    state_q <= state_next;
  end
end

always_comb begin
  state_next = state_q;

  case (state_q)
    ST_IDLE : begin
      if (|cmd_i) begin
        state_next = ST_SETUP;
      end
    end
    ST_SETUP : begin
      state_next = ST_ACCESS;
    end
    ST_ACCESS : begin
      if (pready_i) begin
        state_next = ST_IDLE;
      end
    end
    default : begin
      state_next = state_q;
    end
  endcase // case (state_q)
end // always_comb

assign psel_o    = (state_q == ST_SETUP) | (state_q == ST_ACCESS);
assign penable_o = (state_q == ST_ACCESS);
assign pwrite_o  = cmd_i[1];
assign paddr_o   = 32'hdeadcafe;
assign pwdata_o  = rdata_q + 32'h1;

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    rdata_q <= 32'h0;
  end else begin
    rdata_q <= prdata_i;
  end
end

endmodule // apb_master
