class sample_slave_sequencer extends uvm_sequencer #(sample_seq_item);
  `uvm_component_utils(sample_slave_sequencer)
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction

  task run_phase(uvm_phase phase);
    uvm_report_info("SEQR", "Hi");
  endtask
endclass
