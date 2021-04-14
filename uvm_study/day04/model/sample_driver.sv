class sample_driver extends uvm_driver;
  `uvm_component_utils(sample_driver)
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction
  task run_phase(uvm_phase phase);
    uvm_report_info("DRIVER", "Hi");
  endtask
endclass
