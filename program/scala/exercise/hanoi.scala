object Main extends App {
  val x = args(0).toInt
  def hanoi(x: Int, from: Int, middle:Int, to:Int) {
    if (x > 1) {
      hanoi (x-1, from, to, middle)
      printf("%c --> %c\n", from, to)
      hanoi (x-1, middle, from, to)
    } else {
      printf("%c --> %c\n", from, to)
    }
  }
  hanoi (x, 'A', 'B', 'C')
}



