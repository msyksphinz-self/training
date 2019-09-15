trait Shape {
  def sides: Int
  def perimeter: Double
  def area: Double
}


trait Rectangular extends Shape {
  def width: Double
  def height: Double
  val sides = 4
  override val perimeter = 2*width + 2*height
  override val area = width * height
}


class Circle(radius: Double) extends Shape
{
  val sides = 1
  val perimeter = radius * math.Pi
  val area = (radius / 2) * (radius / 2) * math.Pi
}


class Rectangle(val width: Double, val height: Double) extends Rectangular

class Square(size: Double) extends Rectangular
{
  val width = size
  val height = size
}
