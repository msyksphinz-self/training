// 4.5.6.1 Traffic Lights

sealed trait TrafficLight1 {
  def next: TrafficLight1 =
    this match {
      case Red1    => Green1
      case Green1  => Yellow1
      case Yellow1 => Red1
    }
}

case object Red1 extends TrafficLight1
case object Green1 extends TrafficLight1
case object Yellow1 extends TrafficLight1


sealed trait TrafficLight2 {
  def next: TrafficLight2
}

case object Red2 extends TrafficLight2 {
  def next: TrafficLight2 = Green2
}
case object Green2 extends TrafficLight2 {
  def next: TrafficLight2 = Yellow2
}
case object Yellow2 extends TrafficLight2 {
  def next: TrafficLight2 = Red2
}


// 4.5.6.2 Calculaঞon

sealed trait Calculation
/* final */case class Success(result: Int) extends Calculation
/* final */case class Failure(reason: String) extends Calculation

object Calculator {
  def +(in1: Calculation, in2: Int): Calculation = {
    in1 match {
      case Success(result) => Success(result + in2)
      case Failure(reason) => Failure(reason)
    }
  }
  def -(in1: Calculation, in2: Int): Calculation = {
    in1 match {
      case Success(result) => Success(result - in2)
      case Failure(reason) => Failure(reason)
    }
  }
  def /(in1: Calculation, in2: Int): Calculation = {
    in1 match {
      case Success(result) =>
        in2 match {
          case 0 => { Failure("Division by zero") }
          case _ => { Success(result - in2) }
        }
      case Failure(reason) => Failure(reason)
    }
  }
}


assert(Calculator.+(Success(1), 1) == Success(2))
assert(Calculator.-(Success(1), 1) == Success(0))
assert(Calculator.+(Failure("Badness"), 1) == Failure("Badness"))


assert(Calculator./(Success(4), 2) == Success(2))
assert(Calculator./(Success(4), 0) == Failure("Division by zero"))
assert(Calculator./(Failure("Badness"), 0) == Failure("Badness"))
