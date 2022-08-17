module parallel_serial
  (
   input logic         clk,
   input logic         reset,

   output logic        empty_o,
   input logic         parallel_vld_i,
   input logic [ 3: 0] parallel_i,

   output logic        serial_o,
   output logic        valid_o
   );


logic [ 3: 0]   shift_ff;
logic [ 3: 0]   shift_next;
logic [ 3: 0]   valid_ff;
logic [ 3: 0]   valid_next;


always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    shift_ff <= 'h0;
    valid_ff <= 'h0;
  end else begin
    shift_ff <= shift_next;
    valid_ff <= valid_next;
  end
end

assign serial_o = shift_ff[0];
assign empty_o  = ~|valid_ff;

assign shift_next = parallel_vld_i ? parallel_i :
                    {1'b0, shift_ff[ 3: 1]};
assign valid_next = parallel_vld_i ? 4'b1111 :
                    {1'b0, valid_ff[ 3: 1]};

endmodule // parallel_serial
