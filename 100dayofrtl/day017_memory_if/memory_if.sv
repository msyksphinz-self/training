module memory_if
  (
   input logic          clk,
   input logic          reset,

   input logic          req_i,
   input logic          req_rnw_i,
   input logic [ 3: 0]  req_addr_i,
   input logic [31: 0]  req_wdata_i,
   output logic         req_ready_o,
   output logic [31: 0] req_rdata_o
   );

logic [31: 0]   mem[16];

logic           mem_rd;
logic           mem_wr;

logic           req_rising_edge;

logic [ 3: 0]   lfsr_val;
logic [ 3: 0]   count;
logic [ 3: 0]   count_ff;
logic [ 3: 0]   count_next;

assign mem_rd = req_i &  req_rnw_i;
assign mem_wr = req_i & ~req_rnw_i;

always_ff @ (posedge clk, negedge reset) begin
  if (reset) begin
    count_ff <= 4'h0;
  end else begin
    count_ff <= count_next;
  end
end
assign count_next = req_rising_edge ? lfsr_val :
                    count_ff + 4'h1;

assign count = count_ff;

edge_detector
u_edge_detector
  (
   .clk (clk),
   .reset (reset),
   .a_i (req_i),
   .rising_edge_o (req_rising_edge),
   .falling_edge_o ()
   );


lfsr
u_lfsr
  (
   .clk (clk),
   .reset (reset),
   .lfsr_o (lfsr_val)
   );


always_ff @ (posedge clk) begin
  if (mem_wr & (count == 'h0)) begin
    mem[req_addr_i] <= req_wdata_i;
  end
end

assign req_rdata_o = mem[req_addr_i];

assign req_ready_o = (count == 'h0);

endmodule // memory_if
