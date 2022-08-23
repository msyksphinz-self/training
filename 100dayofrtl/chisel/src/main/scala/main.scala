import chisel3._
import chisel3.util._
import chisel3.stage.{ChiselStage, ChiselGeneratorAnnotation}

// object Main extends App{
//   (new chisel3.stage.ChiselStage).execute(
//     Array("-X", "verilog"),
//     Seq(ChiselGeneratorAnnotation(() => new FirFilter(8, Seq(1.U, 1.U, 1.U)))))
// }

// object Main extends App{
//   (new chisel3.stage.ChiselStage).execute(
//     Array("-X", "verilog"),
//     Seq(ChiselGeneratorAnnotation(() => new Mux())))
// }

object Main extends App{
  (new chisel3.stage.ChiselStage).execute(
    Array("-X", "verilog"),
    Seq(ChiselGeneratorAnnotation(() => new EdgeDetector())))
}
