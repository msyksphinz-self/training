sealed trait IntList {
  def length: Int = {
    this match {
      case End          => 0
      case Pair(hd, tl) => tl.length + 1
    }
  }

  def product: Int = {
    this match {
      case End          => 1
      case Pair(hd, tl) => hd * tl.product
    }
  }
  def double: IntList = {
    this match {
      case End => End
      case Pair(hd, tl) => new Pair(hd * 2, tl.double)
    }
  }
}
case object End extends IntList
case class Pair(head: Int, tail: IntList) extends IntList
val example = Pair(1, Pair(2, Pair(3, End)))


assert(example.length == 3)
assert(example.tail.length == 2)
assert(End.length == 0)

assert(example.product == 6)
assert(example.tail.product == 6)
assert(End.product == 1)

assert(example.double == Pair(2, Pair(4, Pair(6, End))))
assert(example.tail.double == Pair(4, Pair(6, End)))
assert(End.double == End)

sealed trait Tree {
  def sum: Int = {
    this match {
      case Leaf(e)    => e
      case Node(l, r) => l.sum + r.sum
    }
  }
}
case class Node(l: Tree, r: Tree) extends Tree
case class Leaf(elt: Int) extends Tree

// 4.7.0.1 A Calculator
sealed trait Calculation

case class Success(result: Double) extends Calculation
case class Failure(reason: String) extends Calculation

sealed trait Expression {
  def eval: Calculation = {
    this match {
      case Addition(l, r) =>
        l.eval match {
          case Failure(reason) => Failure(reason)
          case Success(r1) =>
            r.eval match {
              case Failure(reason) => Failure(reason)
              case Success(r2) => Success(r1 + r2)
            }
        }
      case Subtraction(l, r) =>
        l.eval match {
          case Failure(reason) => Failure(reason)
          case Success(r1) =>
            r.eval match {
              case Failure(reason) => Failure(reason)
              case Success(r2) => Success(r1 - r2)
            }
        }
      case Division(l , r) =>
        l.eval match {
          case Failure(reason) => Failure(reason)
          case Success(r1) =>
            r.eval match {
              case Failure(reason) => Failure(reason)
              case Success(r2) =>
                if (r2 == 0)
                  Failure("Division by zero")
                else
                  Success(r1 - r2)
            }
        }
      case SquareRoot(l) =>
        l.eval match {
          case Success(r) =>
            if (r < 0)
              Failure("Square root of negative number")
            else
              Success(Math.sqrt(r))
          case Failure(reason) => Failure(reason)
        }
      case Number(v) => Success(v)
    }
  }
}

case class Addition(l: Expression, r: Expression) extends Expression
case class Subtraction(l: Expression, r: Expression) extends Expression
case class Division(l: Expression, r: Expression) extends Expression
case class SquareRoot(l: Expression) extends Expression
case class Number(value: Double) extends Expression
