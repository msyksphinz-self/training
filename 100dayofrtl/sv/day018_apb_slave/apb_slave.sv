module apb_slave
  (
   input logic          clk,
   input logic          reset,

   input logic          psel_i,
   input logic          penable_i,
   input logic [ 3: 0]  paddr_i,
   input logic          pwrite_i,
   input logic [31: 0]  pwdata_i,

   output logic [31: 0] prdata_o,
   output logic         pready_o
   );


logic                   apb_req;
assign apb_req = psel_i & penable_i;

memory_if
u_memory_if
  (
   .clk         ( clk      ),
   .reset       ( reset    ),
   .req_i       ( apb_req  ),
   .req_rnw_i   (~pwrite_i ),
   .req_addr_i  ( paddr_i  ),
   .req_wdata_i ( pwdata_i ),
   .req_ready_o ( pready_o ),
   .req_rdata_o ( prdata_o )
   );

endmodule // apb_slave
