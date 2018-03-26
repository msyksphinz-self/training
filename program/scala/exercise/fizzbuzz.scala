object Main extends App {
  def fizzbuzz(n: Int) =
    if (n % 15 == 0) { "FizzBuzz" }
    else if (n % 5 == 0) { "Buzz" }
    else if (n % 3 == 0) { "Fizz" }
    else { n.toString }

  (1 to 100).map(fizzbuzz).foreach(println)
//  for (i <- 1 to 100) {
//    var is_printed = 0
//    if (i % 3 == 0) {
//      print("Fizz")
//      is_printed = 1
//    }
//    if (i % 5 == 0) {
//      print("Buzz")
//      is_printed = 1
//    }
//    if (is_printed == 0) {
//      print(i)
//    }
//    print(" ")
//  }
}


