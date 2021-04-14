class test extends uvm_test;
  `uvm_component_utils(test)
  `uvm_new_func
    task run_phase(uvm_phase phase);
      phase.raise_objection(this);
      $display("Hello World\n");
      phase.drop_objection(this);
    endtask
endclass
