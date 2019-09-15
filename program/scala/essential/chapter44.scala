sealed trait TrafficLight
case object Red extends TrafficLight
case object Green extends TrafficLight
case object Yellow extends TrafficLight

sealed trait Calculation
final case class Sucess(result: Int) extends Calculation
final case class Failure(reason: String) extends Calculation

sealed trait Source
case object well extends Source
case object spring extends Source
case object tap extends Source

final case class BottledWater(
  size: Int,
  source: Source,
  is_carbonated: Boolean)
