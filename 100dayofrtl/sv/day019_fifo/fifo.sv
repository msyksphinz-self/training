module fifo
  #(
    parameter DEPTH = 4,
    parameter DATA_W = 1
    )
(
 input logic clk,
 input logic reset,

 input logic push_i,
 input logic [DATA_W-1: 0] push_data_i,

 input logic pop_i,
 output logic [DATA_W-1: 0] pop_data_o,

 output logic full_o,
 output logic empty_o
 );


localparam PTR_W = $clog2(DEPTH);


logic [PTR_W: 0] rd_ptr_next;
logic [PTR_W: 0] rd_ptr_q;

logic [PTR_W: 0] wr_ptr_next;
logic [PTR_W: 0] wr_ptr_q;

logic [DATA_W-1: 0] fifo_pop_data;

assign pop_data_o = fifo_pop_data;


logic [DATA_W-1: 0]  fifo_mem [DEPTH];

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    rd_ptr_q <= 'h0;
    wr_ptr_q <= 'h0;
  end else begin
    rd_ptr_q <= rd_ptr_next;
    wr_ptr_q <= wr_ptr_next;
  end
end

always_comb begin
  rd_ptr_next = rd_ptr_q;
  wr_ptr_next = wr_ptr_q;

  case ({pop_i, push_i})
    2'b01 : begin
      wr_ptr_next = wr_ptr_q + 'h1;
    end
    2'b10 : begin
      rd_ptr_next = rd_ptr_q + 'h1;
      fifo_pop_data = fifo_mem[rd_ptr_q[PTR_W-1: 0]];
    end
    2'b11 : begin
      wr_ptr_next = wr_ptr_q + 'h1;
      rd_ptr_next = rd_ptr_q + 'h1;
    end
    default : begin
      wr_ptr_next = wr_ptr_q;
      rd_ptr_next = rd_ptr_q;
    end
  endcase // case ({pop_i, push_i})
end // always_comb

always_ff @ (posedge clk) begin
  if (push_i) begin
    fifo_mem[wr_ptr_q[PTR_W-1: 0]] <= push_data_i;
  end
end

assign full_o = (rd_ptr_q[PTR_W] != wr_ptr_q[PTR_W]) &
                (rd_ptr_q[PTR_W-1: 0] == wr_ptr_q[PTR_W-1: 0]);

assign empty_o = (rd_ptr_q[PTR_W: 0] == wr_ptr_q[PTR_W: 0]);

endmodule // fifo
