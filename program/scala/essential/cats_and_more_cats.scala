trait Feline {
  def colour: String
  def sound: String
}

case class Cats   (colour:String, food:String) extends Feline
{
  val sound = "meow"
}
case class Tigers (colour:String) extends Feline
{
  val sound = "roar"
}
case class Lions  (colour:String, maneSize:Int) extends Feline
{
  val sound = "roar"
}

case class Panther(colour:String) extends Feline
{
  val sound = "roar"
}
