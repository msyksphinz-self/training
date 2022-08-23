import chisel3._
import chisel3.util._

class RoundRobin (NumPorts: Int = 4)extends Module {
  val io = IO(new Bundle {
    val req = Input (UInt(NumPorts.W))
    val gnt = Output(UInt(NumPorts.W))
  })


  var r_mask = RegInit(((1 << NumPorts)-1).U(NumPorts.W))
  for (i <- 0 to NumPorts-1) {
    when (io.gnt(i)) {
      r_mask := (((1 << NumPorts)-1) << i).U
    }
  }

  val w_mask_req = Wire (UInt(NumPorts.W))
  w_mask_req := io.req & r_mask

  val w_mask_gnt = Wire (Vec(NumPorts, Bool()))

  val u_mask_gnt = Module (new PriArbiter(NumPorts))
  u_mask_gnt.io.req := w_mask_req.asBools;
  w_mask_gnt        := u_mask_gnt.io.gnt

  val w_raw_gnt  = Wire (Vec(NumPorts, Bool()))
  val u_raw_gnt  = Module (new PriArbiter(NumPorts))
  u_raw_gnt.io.req := w_mask_req.asBools;
  w_raw_gnt        := u_raw_gnt.io.gnt

  io.gnt := Mux (w_mask_req.orR, w_mask_gnt, w_raw_gnt).asUInt

}
