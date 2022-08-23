import chisel3._
import chisel3.util._

class Mux extends Module {
  val io = IO(new Bundle {
    val a_i = Input (UInt(8.W))
    val b_i = Input (UInt(8.W))
    val sel_i = Input (Bool())
    val y_o = Output (UInt(8.W))
  })

  io.y_o := Mux (io.sel_i, io.a_i, io.b_i)
}
