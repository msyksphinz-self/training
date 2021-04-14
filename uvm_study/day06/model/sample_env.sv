class sample_env extends uvm_env;
  `uvm_component_utils(sample_env)
  sample_master_agent master;
  sample_slave_agent  slave;

  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction // new

  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    master = sample_master_agent::type_id::create("master", this);
    slave  = sample_slave_agent::type_id::create("slave", this);
  endfunction // build_phase

  task run_phase(uvm_phase phase);
    uvm_report_info("ENV", "Hello ENV");
  endtask
endclass
