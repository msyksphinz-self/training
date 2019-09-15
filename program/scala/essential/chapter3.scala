// Exercise 3.1.6.1

class Cats(val Name: String, val Colour: String, val Food: String)

val oswald    = new Cats("Oswald", "Black", "Milk")
val henderson = new Cats("Henderson", "Ginger", "Chips")
val queen     = new Cats("Queen", "Tabby and white", "Curry")

// Exercise 3.1.6.2

object ChipShop {
  def willServe(cat: Cats): Boolean = {
    if (cat.Food == "Chips") {
      return true
    } else {
      return false
    }
  }
}


assert(ChipShop.willServe(oswald) == false)
assert(ChipShop.willServe(henderson) == true)
assert(ChipShop.willServe(queen) == false)

// Exercise 3.1.6.3
class Director(val firstName: String, val lastName: String, val yearOfBirth: Int)
{
  def name(): String = {
    return firstName + " " + lastName
  }
}
class Film (val name: String, val yearOfRelease: Int, val imdbRating: Double, val director: Director)
{
  def directorsAge(): Int = {
    return yearOfRelease - director.yearOfBirth
  }
  def isDirectedBy(director: Director): Boolean = {
    return director.name() == name
  }
  def copy(name: String = this.name, yearOfRelease: Int = this.yearOfRelease, imdbRating: Double = this.imdbRating, director: Director = this.director): Film = {
    return new Film(name, yearOfRelease, imdbRating, director)
  }
}

val eastwood = new Director("Clint", "Eastwood", 1930)
val mcTiernan = new Director("John", "McTiernan", 1951)
val nolan = new Director("Christopher", "Nolan", 1970)
val someBody = new Director("Just", "Some Body", 1990)
val memento = new Film("Memento", 2000, 8.5, nolan)
val darkKnight = new Film("Dark Knight", 2008, 9.0, nolan)
val inception = new Film("Inception", 2010, 8.8, nolan)
val highPlainsDrifter = new Film("High Plains Drifter", 1973, 7.7, eastwood)
val outlawJoseyWales = new Film("The Outlaw Josey Wales", 1976, 7.9, eastwood)
val unforgiven = new Film("Unforgiven", 1992, 8.3, eastwood)
val granTorino = new Film("Gran Torino", 2008, 8.2, eastwood)
val invictus = new Film("Invictus", 2009, 7.4, eastwood)
val predator = new Film("Predator", 1987, 7.9, mcTiernan)
val dieHard = new Film("Die Hard", 1988, 8.3, mcTiernan)
val huntForRedOctober = new Film("The Hunt for Red October", 1990,7.6, mcTiernan)
val thomasCrownAffair = new Film("The Thomas Crown Affair", 1999, 6.8, mcTiernan)

assert(eastwood.yearOfBirth == 1930)
assert(dieHard.director.name == "John McTiernan")
assert(invictus.isDirectedBy(nolan) == false)

highPlainsDrifter.copy(name = "L'homme des hautes plaines")
// res19: Film = Film(L'homme des hautes plaines,1973,7.7,Director(Clint,Eastwood,1930))
thomasCrownAffair.copy(yearOfRelease = 1968,
  director = new Director("Norman", "Jewison", 1926))
// res20: Film = Film(The Thomas Crown Affair,1968,6.8,Director(Norman,Jewison,1926))
inception.copy().copy().copy()
// res21: Film = Film(Inception,2010,8.8,Director(Christopher,Nolan,1970))


// Exercise 3.1.6.4
class Counter(val count: Int) {
  def inc(): Counter = { new Counter(count + 1) }
  def dec(): Counter = { new Counter(count - 1) }
}

assert(new Counter(10).inc.dec.inc.inc.count == 12)

// Exercise 3.1.6.5
class CounterFast(val count: Int) {
  def inc(v: Int = 1): CounterFast = { new CounterFast(count + v) }
  def dec(v: Int = 1): CounterFast = { new CounterFast(count - v) }
}

assert(new CounterFast(10).inc().dec().inc(2).count == 12)

// Exercise 3.1.6.6
class Adder(amount: Int) {
  def add(in: Int) = in + amount
}
class CounterAdder(val count: Int) {
  def dec = new CounterAdder(count - 1)
  def inc = new CounterAdder(count + 1)
  def adjust(adder: Adder): CounterAdder = new CounterAdder(adder.add(count))
}

assert(new CounterAdder(5).adjust(new Adder(10)).count == 15)

// Exercise 3.3.2.1
class Person(val first: String, val last: String)
object Person {
  def apply(name: String): Person = {
    val splitted_name = name.split(" ")
    new Person(splitted_name(0), splitted_name(1))
  }
}

// 3.3.2.2 Extended Body of Work

object Director {
  def apply(firstName: String, lastName: String, yearOfBirth: Int): Director = {
    new Director(firstName, lastName, yearOfBirth)
  }
  def older(first: Director, second: Director): Director = {
    if (first.yearOfBirth < second.yearOfBirth) {
      first
    } else {
      second
    }
  }
}

object Film {
  def apply(name: String,
    yearOfRelease: Int,
    imdbRating: Double,
    director: Director): Film = {
    new Film(name, yearOfRelease, imdbRating, director)
  }
  def higestRating (first: Film, second: Film): Film = {
    if (first.imdbRating > second.imdbRating) {
      first
    } else {
      second
    }
  }
  def oldestDirectorAtTheTime(first: Film, second: Film): Film = {
    if (first.yearOfRelease < second.yearOfRelease) {
      first
    } else {
      second
    }
  }
}
