class sample_test extends uvm_test;
  `uvm_component_utils(sample_test)

  sample_env env;
  function new (string name="sample_test", uvm_component parent=null);
    super.new(name, parent);
  endfunction // new

  function void build_phase (uvm_phase phase);
    super.build_phase(phase);
    env = sample_env::type_id::create("env", this);
  endfunction // build_phase

  task run_phase(uvm_phase phase);
    uvm_report_info("TEST", "Hello World");
  endtask
endclass // sample_test


class sample_seq_item extends uvm_sequence_item;
  rand byte addr, data;
  bit       write;
  `uvm_object_utils(sample_seq_item)
  function new (string name="sample_seq_item_inst");
    super.new(name);
  endfunction // new
endclass // sample_seq_item
