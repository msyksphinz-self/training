class sample_test extends uvm_test;
  `uvm_component_utils(sample_test)

  sample_env env;
  function new (string name="sample_test", uvm_component parent=null);
    super.new(name, parent);
  endfunction // new

  function void build_phase (uvm_phase phase);
    super.build_phase(phase);
    uvm_config_db#(uvm_object_wrapper)::set(this,
        "env.master.sequencer.run_phase", "default_sequence",
        write_seq::type_id::get());
    uvm_config_db#(uvm_object_wrapper)::set(this,
        "env.slave.sequencer.run_phase", "default_sequence",
        normal_response_seq::type_id::get());
    env = sample_env::type_id::create("env", this);
  endfunction // build_phase

  task run_phase(uvm_phase phase);
    uvm_report_info("TEST", "Hello World");
    uvm_top.print_topology();
  endtask
endclass // sample_test
