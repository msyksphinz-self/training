fn power (n: f64, m: i32) -> f64 {
    match m {
        0 => 1.0,
        1 => n,
        _ => {
            let x = power(n, m / 2);
            if m % 2 == 0 {
                x * x
            } else {
                x * x * n
            }
        }
    }
}


fn main() {
    for i in 0 .. 10 {
        println!("{}", power(2.0, i))
    }
}
