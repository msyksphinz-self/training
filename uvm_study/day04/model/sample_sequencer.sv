class sample_sequencer extends uvm_sequencer;
  `uvm_component_utils(sample_sequencer)
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction
  task run_phase(uvm_phase phase);
    uvm_report_info("SEQR", "Hi");
  endtask
endclass
