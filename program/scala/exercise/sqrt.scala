object Main extends App {
  val A = args(0).toFloat
  var num = 1.0
  for (i <- 1 to 10) {
    num = (num + A / num) / 2.0
  }
  printf("%f\n", num)
}
