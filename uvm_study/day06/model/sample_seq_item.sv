class sample_seq_item extends uvm_sequence_item;
  rand byte addr, wdata, rdata;
  bit       write;
  bit [3:0] wait_cycle;
  `uvm_object_utils(sample_seq_item)
  function new (string name="sample_seq_item_inst");
    super.new(name);
  endfunction // new
endclass // sample_seq_item
