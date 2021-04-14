class sample_agent extends uvm_agent;
  `uvm_component_utils(sample_agent)
  sample_driver    driver;
  sample_monitor   monitor;
  sample_sequencer sequencer;
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction
  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    driver    = sample_driver::type_id::create("driver", this);
    monitor   = sample_monitor::type_id::create("monitor", this);
    sequencer = sample_sequencer::type_id::create("sequencer", this);
  endfunction
  task run_phase(uvm_phase phase);
    uvm_report_info("AGENT", "Hi");
  endtask
endclass
