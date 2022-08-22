typedef struct packed {
  logic          valid;
  logic [15: 0]  addr;
} mem_req_req_t;
typedef struct packed {
logic          ready;
} mem_req_ack_t;

typedef struct packed {
logic          valid;
logic [31: 0]  data;
} mem_data_resp_t;

typedef struct packed {
logic          ready;
} mem_data_ack_t;

module rdmemory
  (
   input               i_clk,
   input               i_reset_n,

   input logic         i_master_valid,
   output logic        i_master_ready,
   input logic [15: 0] i_master_addr,

   output logic        o_slave_valid,
   input logic         i_slave_ready,
   output [31: 0]      o_slave_data,

   output              mem_req_req_t mem_req,
   input               mem_req_ack_t mem_req_ack,
   input               mem_data_resp_t mem_resp,
   output              mem_data_ack_t mem_data_ack
   );

sig v_req_fifo
  (
   .i_clk     (i_clk),
   .i_reset_n (i_reset),
   .o_stall (),

   );

fifo u_data_fifo
(
 .i_clk (i_clk),
 .i_reset_n (i_reset_n),

 .i_master_valid (),
 .i_master_data  (),
 .o_master_ready (),
 .o_slave_valid  (),
 .o_slave_data   (),
 .i_slave_ready  ()
 );

endmodule // rdmemory
