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

class Rectangle(width: Double, height: Double) extends Shape
{
  val sides = 4
  val perimeter = width * 2 + height * 2
  val area = width * height
}

class Square(size: Double) extends Shape
{
  val sides = 4
  val perimeter = size * 4
  val area = size * size
}
