class sample_monitor extends uvm_monitor;
  `uvm_component_utils(sample_monitor)
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction
  task run_phase(uvm_phase phase);
    uvm_report_info("MONITOR", "Hi");
  endtask
endclass
