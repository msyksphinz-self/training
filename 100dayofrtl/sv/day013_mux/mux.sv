module mux
  (
   input logic [ 3: 0] a_i,
   input logic [ 3: 0] sel_i,

   output logic y_ter_o,
   output logic y_case_o,
   output logic y_ifelse_o,
   output logic y_loop_o,
   output logic y_aor_o
   );


assign y_ter_o = sel_i[0] ? a_i[0] :
                 sel_i[1] ? a_i[1] :
                 sel_i[2] ? a_i[2] :
                 a_i[3];

always_comb begin
  case (sel_i)
    4'b0001 : y_case_o = a_i[0];
    4'b0010 : y_case_o = a_i[1];
    4'b0100 : y_case_o = a_i[2];
    4'b1000 : y_case_o = a_i[3];
    default : y_case_o = 1'bx;
  endcase // case (sel_i)
end

always_comb begin
  if      (sel_i[0]) begin y_ifelse_o = a_i[0]; end
  else if (sel_i[1]) begin y_ifelse_o = a_i[1]; end
  else if (sel_i[2]) begin y_ifelse_o = a_i[2]; end
  else if (sel_i[3]) begin y_ifelse_o = a_i[3]; end
  else               begin y_ifelse_o = 1'bx;   end
end

always_comb begin
  y_loop_o = 'h0;
  for (int i = 0; i < 4; i++) begin
    y_loop_o = (sel_i[i] & a_i[1]) | y_loop_o;
  end
end

assign y_aor_o = (sel_i[0] & a_i[0]) |
                 (sel_i[1] & a_i[1]) |
                 (sel_i[2] & a_i[2]) |
                 (sel_i[3] & a_i[3]) |
                 1'b0;

endmodule // mux
