object Main extends App {
  var num = args(0).toInt
  for (i <- 2 to num-1) {
    if (num % i == 0) {
      printf("%d is not prime number\n", num);
      sys.exit
    }
  }
  printf("%d is prime number\n", num);
}

