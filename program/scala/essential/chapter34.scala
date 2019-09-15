// 3.4.5.1 Case Cats
case class Cats(val Name: String, val Colour: String, Food: String)

// 3.4.5.2 Roger Ebert Said it Best…
case class Director(firstName: String, lastName: String, yearOfBirth:Int) {
  def name: String =
    s"$firstName $lastName"
}
object Director {
  def older(director1: Director, director2: Director): Director =
    if (director1.yearOfBirth < director2.yearOfBirth) director1 else director2
}

case class Film(
  name: String,
  yearOfRelease: Int,
  imdbRating: Double,
  director: Director) {
  def directorsAge =
    yearOfRelease - director.yearOfBirth
  def isDirectedBy(director: Director) =
    this.director == director
}

object Film {
  def newer(film1: Film, film2: Film): Film =
    if (film1.yearOfRelease < film2.yearOfRelease) film1 else film2
  def highestRating(film1: Film, film2: Film): Double = {
    val rating1 = film1.imdbRating
    val rating2 = film2.imdbRating
    if (rating1 > rating2) rating1 else rating2
  }
  def oldestDirectorAtTheTime(film1: Film, film2: Film): Director =
    if (film1.directorsAge > film2.directorsAge) film1.director else
                                                                  film2.director
}


// 3.4.5.3 Case Class Counter
case class Counter(val count: Int = 0) {
  def inc(): Counter = { this.copy(count + 1) }
  def dec(): Counter = { this.copy(count - 1) }
}

case class Person(val first: String, val last: String)
object Person {
  def apply(name: String): Person = {
    val splitted_name = name.split(" ")
    new Person(splitted_name(0), splitted_name(1))
  }
}


// 3.5.3.1 Feed the Cats

object ChipShop {
  def willServe(cat: Cats): Boolean = {
    cat.Food match {
      case "Chips" => true
      case _       => false
    }
  }
}

// 3.5.3.2 Get Off My Lawn!

object Dad {
  def accepts(film: Film): Double = {
    film.director.name match {
      case "Clint Eastwood" => 10.0
      case "John McTiernan" => 7.0
      case _ => 3.0
    }
  }
}
