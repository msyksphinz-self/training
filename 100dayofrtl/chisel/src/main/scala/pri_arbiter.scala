import chisel3._
import chisel3.util._



class PriArbiter (NumPorts: Int = 4)extends Module {
  val io = IO(new Bundle {
    val req = Input (Vec(NumPorts, Bool()))
    val gnt = Output(Vec(NumPorts, Bool()))
  })

  io.gnt(0) := io.req(0)
  for (i <- 1 to NumPorts-1) {
    var tmp = io.req(0)
    for (j <- 0 to i-1) {
      tmp = tmp | io.req(j)
    }
    io.gnt(i) := io.req(i) & tmp
  }

}
