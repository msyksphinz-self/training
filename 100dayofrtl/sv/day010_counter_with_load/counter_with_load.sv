module counter_with_load
  (
   input logic          clk,
   input logic          reset,

   input logic          load_i,
   input logic [ 3: 0]  load_val_i,

   output logic [ 3: 0] count_o
   );


logic [ 3: 0] count_ff;
logic [ 3: 0] count_next;

always_ff @ (posedge clk, posedge reset) begin
  if (reset) begin
    count_ff <= 4'h0;
  end else begin
    count_ff <= count_next;
  end
end

assign count_next = load_i ? load_val_i :
                    count_ff + 4'h1;

assign count_o = count_ff;

endmodule // counter_with_load
