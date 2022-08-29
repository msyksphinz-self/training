module fifo
  #(
    parameter D_WIDTH = 32,
    parameter LENGTH = 8
    )
(
 input logic i_clk,
 input logic i_reset_n,

 input  logic i_master_valid,
 output logic o_master_ready,
 input logic [D_WIDTH-1: 0] i_master_data,

 output logic o_slave_valid,
 input  logic i_slave_ready,
 output logic [D_WIDTH-1: 0] o_slave_data
 );

logic [D_WIDTH-1: 0]         r_mem[LENGTH];

logic [$clog2(LENGTH): 0]  r_input_ptr;
logic [$clog2(LENGTH): 0]  r_output_ptr;

logic                      w_alloc;
logic                      w_dealloc;
assign w_alloc   = i_master_valid & o_master_ready;
assign w_dealloc = o_slave_valid  & i_slave_ready;

logic                      w_full;
logic                      w_empty;

always_ff @ (posedge i_clk, negedge i_reset_n) begin
  if (!i_reset_n) begin
    r_input_ptr  <= {($clog2(LENGTH) + 1){1'b0}};
    r_output_ptr <= {($clog2(LENGTH) + 1){1'b0}};
  end else begin
    if (w_alloc & !w_full) begin
      r_input_ptr <= r_input_ptr + 'h1;
    end
    if (w_dealloc & !w_empty) begin
      r_output_ptr <= r_output_ptr + 'h1;
    end
  end // else: !if(!i_reset_n)
end // always_ff @ (posedge i_clk, negedge i_reset_n)

assign w_full = (r_input_ptr[$clog2(LENGTH)] != r_output_ptr[$clog2(LENGTH)]) &
                (r_input_ptr[$clog2(LENGTH)-1: 0] == r_output_ptr[$clog2(LENGTH)-1: 0]);
assign w_empty = r_input_ptr == r_output_ptr;

always_ff @ (posedge i_clk) begin
  if (w_alloc & !w_full) begin
    r_mem[r_input_ptr[$clog2(LENGTH)-1: 0]] <= i_master_data;
  end
end

assign o_slave_data = r_mem[r_output_ptr[$clog2(LENGTH)-1: 0]];

endmodule // fifo
