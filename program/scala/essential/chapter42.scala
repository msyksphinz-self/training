// 4.2.2.1 Prinঞng Shapes

sealed trait Shape {
  def sides: Int
  def perimeter: Double
  def area: Double
}


case class Circle(val radius: Double) extends Shape
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


object Draw {
  def apply(shape: Shape): String = shape match {
    case Circle(radius) =>
      s"A circle of radius " + radius
    case Rectangle(width, height) =>
      s"A rectanglar of width " + width + "cm and height " + height + "cm"
    case Square(size) =>
      s"A square of width " + size + "cm"
  }
}

println(Draw(Circle(10)))
println(Draw(Rectangle(3, 4)))
