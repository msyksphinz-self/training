fn ninety_nine() {
    println!("  |  1  2  3  4  5  6  7  8  9");
    println!("------------------------------");
    for x in 1..10 {
        print!("{} | ", x);
        for y in 1..10 {
            print! ("{:>2} ", x * y);
        }
        println!("");
    }
    println!("");
}

fn main() {
    ninety_nine();
}
