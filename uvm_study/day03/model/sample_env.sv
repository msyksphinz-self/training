class sample_env extends uvm_env;
  `uvm_component_utils(sample_env)
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction
  task run_phase(uvm_phase phase);
    uvm_report_info("ENV", "Hello ENV");
  endtask
endclass
