class sample_slave_monitor extends uvm_monitor;
  virtual sample_if vif;
  `uvm_component_utils(sample_slave_monitor)
  function new (string name, uvm_component parent);
    super.new(name, parent);
  endfunction
  function void build_phase(uvm_phase phase);
    bit status;
    super.build_phase(phase);
    status = uvm_config_db#(virtual sample_if)::get(this, "", "vif", vif);
    if(status==1'b0)
      uvm_report_fatal("NOVIF", {"virtual interface must be set for: ",get_full_name(),".vif"});
  endfunction
  task run_phase(uvm_phase phase);
    string s_trans_kind;
    uvm_report_info("MONITOR", "Hi");
    forever begin
      @(posedge vif.valid);
      if(vif.write===1'b1)
        s_trans_kind = "write";
      else if(vif.write===1'b0)
        s_trans_kind = "read";
      else
        s_trans_kind = "Unknown";
      uvm_report_info("MON", $sformatf("%s addr=%02xh data=%02xh", s_trans_kind, vif.addr, vif.data));
    end
  endtask
endclass
