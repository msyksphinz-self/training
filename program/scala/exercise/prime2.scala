object Main extends App {
  def prime(n: Int) =
    2 <= n && !(2 until (math.sqrt(n) + 1.0).toInt).exists(n % _ == 0)

  val n = args(0).toInt

  println(n.toString + "は" + (if (prime(n)) "素数" else "非素数"))
}

