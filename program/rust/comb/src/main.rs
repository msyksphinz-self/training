fn comb(n: i64, r: i64) -> i64 {
    match (n, r) {
        (0, _) | (_, 0) => 1,
        _ => comb(n, r - 1) * (n - r + 1) / r
    }
}

fn main() {
    println!("{}", comb(5, 3));
    println!("{}", comb(20, 10));
    println!("{}", comb(30, 15));
}
