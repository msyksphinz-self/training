// 4.1.4.1 Cats, and More Cats

trait Feline {
  def colour: String
  def sound: String
}

case class Cats (colour: String) extends Feline {
  val sound = "meaw"
}

case class Tiger(colour: String) extends Feline {
  val sound = "roar"
}

case class Lion (colour: String, maneSize: Int) extends Feline {
  val sound = "roar"
}

case class Panther(colour: String) extends Feline {
  val sound = "roar"
}


// 4.1.4.2 Shaping Up With Traits

trait Shape {
  def sides: Int
  def perimeter: Double
  def area: Double
}


class Circle(radius: Double) extends Shape
{
  val sides = 1
  val perimeter = radius * math.Pi
  val area = (radius / 2) * (radius / 2) * math.Pi
}


sealed trait Rectangular extends Shape {
  def width: Double
  def height: Double
  val sides = 4
  val perimeter = width * 2 + height * 2
  val area = width * height
}

case class Rectangle (val width: Double, val height: Double) extends Rectangular

case class Square(size: Double) extends Rectangular
{
  val width = size
  val height = size
}


// 4.1.4.3 Shaping Up 2 (Da Streets)
