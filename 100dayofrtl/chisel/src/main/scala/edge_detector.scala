import chisel3._
import chisel3.util._

class EdgeDetector extends Module {
  val io = IO(new Bundle {
    val a = Input(Bool())
    val rising_edge  = Output(Bool())
    val falling_edge = Output(Bool())
  })

  val a_ff = RegNext(io.a, false.B)

  io.rising_edge  := !a_ff &  io.a
  io.falling_edge :=  a_ff & !io.a

}
