module alu
  (
   input logic [ 7: 0] a_i,
   input logic [ 7: 0] b_i,
   input logic [ 2: 0] op_i,

   output logic [ 7: 0] alu_o
   );

typedef enum logic [ 2: 0]
{
 OP_ADD = 3'b000,
 OP_SUB = 3'b001,
 OP_SLL = 3'b010,
 OP_LSR = 3'b011,
 OP_AND = 3'b100,
 OP_OR  = 3'b101,
 OP_XOR = 3'b110,
 OP_EQL = 3'b111
} op_t;

logic        carry;
always_comb begin
  carry = 1'b0;
  case (op_i)
    OP_ADD : {carry, alu_o} = {1'b0, a_i} + {1'b0, b_i};
    OP_SUB : alu_o = a_i - b_i;
    OP_SLL : alu_o = a_i[ 7: 0] << b_i[ 2: 0];
    OP_LSR : alu_o = a_i[ 7: 0] >> b_i[ 2: 0];
    OP_AND : alu_o = a_i[ 7: 0] &  b_i[ 7: 0];
    OP_OR  : alu_o = a_i | b_i;
    OP_XOR : alu_o = a_i ^ b_i;
    OP_EQL : alu_o = {7'h0, a_i == b_i};
    default : alu_o = 'h0;
  endcase // case (op_i)
end // always_comb

endmodule // alu
