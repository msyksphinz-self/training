object Main extends App {
  val year = args(0).toInt
  def leap_year (year: Int) = 
    if (year % 4 == 0) {
      if (year % 100 == 0) {
        if (year % 400 == 0) { true  }
        else                 { false }
      } else                 { true }
    } else                   { false }

  println(year + " is " + (if (leap_year(year)) "" else "not" + " leap year\n"))
}
