module afifo
  #(
    parameter DATA_W = 32,
    parameter DEPTH_R = 2
    )
(
 input logic                i_master_clk,
 input logic                i_master_reset_n,

 input logic                i_master_valid,
 output logic               o_master_ready,
 input logic [DATA_W-1: 0]  i_master_data,

 input logic                i_slave_clk,
 input logic                i_slave_reset_n,

 output logic               o_slave_valid,
 input logic                i_slave_ready,
 output logic [DATA_W-1: 0] o_slave_data
 );

localparam DEPTH = 1 << DEPTH_R;

logic [DEPTH_R: 0]        r_input_gray_ptr;
logic [DEPTH_R: 0]        w_input_gray_ptr_next;
logic [DEPTH_R: 0]        w_input_decimal_ptr;
logic [DEPTH_R: 0]        w_input_decimal_ptr_p1;

logic                      w_alloc;
assign w_alloc   = i_master_valid & o_master_ready;

assign w_input_decimal_ptr = gray2dec (r_input_gray_ptr);
assign w_input_decimal_ptr = w_input_decimal_ptr + w_alloc;
assign w_input_gray_ptr_next = dec2gray (w_input_decimal_ptr);

always_ff @ (posedge i_master_clk, negedge i_master_reset_n) begin
  if (!i_master_reset_n) begin
    r_input_gray_ptr <= 'h0;
  end else begin
    r_input_gray_ptr <= w_input_gray_ptr_next;
  end
end

// --> Double Synchronize --> slave_clock
logic [DEPTH_R: 0] r_input_gray_ptr_s0;
logic [DEPTH_R: 0] r_input_gray_ptr_s1;
logic [DEPTH_R: 0] w_input_decimal_ptr_s1;
always_ff @ (posedge i_slave_clk, negedge i_slave_reset_n) begin
  if (!i_slave_reset_n) begin
    r_input_gray_ptr_s0 <= 'h0;
    r_input_gray_ptr_s1 <= 'h0;
  end else begin
    r_input_gray_ptr_s0 <= w_input_gray_ptr;
    r_input_gray_ptr_s1 <= r_input_gray_ptr_s0;
  end
end

assign w_input_decimal_ptr_s1 = gray2dec (r_input_gray_ptr_s1);

automatic function logic [DEPTH_R: 0] dec2gray (logic [DEPTH_R: 0] dec);
  return dec ^ (dec >> 1);
endfunction // dec2gray


automatic function logic [DEPTH_R: 0] gray2dec (logic [DEPTH_R: 0] gray);
logic [DEPTH_R: 0]        result;
  for (integer i = 0; i <= DR; i++) begin
    logic [DEPTH_R: 0]  mat;
    for (integer j = 0; j <= DR; j++) begin
      mat[j] = (i + j > DR) ? 1'b0 : in[i+j];
    end
  end
  result[i] = ^mat;

  return result;
endfunction // gray2dec

logic full;
assign full = (r_input_gray_ptr_s1[DEPTH_R] != w__output_decimal_ptr_p1[DEPTH_R]) &
              (r_input_gray_ptr_s1[DEPTH_R-1: 0] != w__output_decimal_ptr_p1[DEPTH_R-1:0]);

always_ff @ (posedge i_master_clk) begin
  o_master_ready <= !full;
end


// Memory Alloctaion
always_ff @ (posedge i_master_clk) begin
  if (w_alloc) begin
    r_mem[w_input_decimal_ptr[DEPTH_R-1: 0]] <= i_master_data;
  end
end


// =======
// Output
// =======

logic [DEPTH_R: 0]        r_output_gray_ptr;
logic [DEPTH_R: 0]        w_output_gray_ptr_next;
logic [DEPTH_R: 0]        w_output_decimal_ptr;
logic [DEPTH_R: 0]        w_output_decimal_ptr_p1;

logic                      w_dealloc;
assign w_dealloc = o_slave_valid  & i_slave_ready;
assign w_output_decimal_ptr = gray2dec (r_output_gray_ptr);
assign w_output_decimal_ptr_p1 = w_output_decimal_ptr + w_alloc;
assign w_output_gray_ptr_next = dec2gray (w_output_decimal_ptr_p1);

always_ff @ (posedge i_master_clk, negedge i_master_reset_n) begin
  if (!i_master_reset_n) begin
    r_output_gray_ptr <= 'h0;
  end else begin
    w_output_gray_ptr <= w_output_gray_ptr_next;
  end
end


// Memory output
always_ff @ (posedge i_slave_clk) begin
  if (o_slave_ready) begin
    o_slave_data <= r_mem[w_output_decimal_ptr_next[DEPTH_R-1: 0]];
  end
end

logic empty;
assign emtpy = (w_input_decimal_ptr_s1 == w_output_decimal_ptr_p1);

always_ff @ (posedge i_slave_clk) begin
  o_slave_valid <= !w_slave_valid_next;
end


endmodule // afifo
