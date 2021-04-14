class sample_master_driver extends uvm_driver #(sample_seq_item);

  virtual sample_if vif;

  `uvm_component_utils(sample_master_driver)
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction

  function void build_phase(uvm_phase phase);
  bit   status;
    super.build_phase(phase);
    status = uvm_config_db #(virtual sample_if)::get(this, "", "vif", vif);
    if (status == 1'b0)
      uvm_report_fatal("NOVIF", {"virtual interface must be set for: ", get_full_name(), ".vif"});
  endfunction // build_phase


  task run_phase(uvm_phase phase);
    uvm_report_info("DRIVER", "Hi");
    vif.valid <= 1'b0;
    uvm_report_info("DRIVER", "wating release reset");
    @(posedge vif.rstz);  // wait reset negate
    forever begin
      seq_item_port.get_next_item(req);  // wait seq_item from sequence (via sequencer)
      uvm_report_info("DRIVER", "waiting clk");
      @(posedge vif.clk); // sync clk
      uvm_report_info("DRIVER", "start request");
      vif.valid <= 1'b1;
      vif.write <= req.write;
      vif.addr  <= req.addr;
      if (req.write == 1'b1) vif.wdata <= req.wdata;
      @(posedge vif.ready);
      if (req.write == 1'b0) req.rdata = vif.rdata;
      @(posedge vif.clk);
      vif.valid <= 1'b0;
      seq_item_port.item_done(rsp);
    end
  endtask

endclass
