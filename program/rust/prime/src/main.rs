// prime number check
fn is_prime(x: i32, ps: &Vec<i32>) -> bool {
    for p in ps {
        if p * p > x { break; }
        if x % p == 0 { return false; }
    }
    true
}

fn prime (n: i32) {
    let mut ps = vec![2];
    let mut x = 3;
    while x <= n {
        if is_prime(x, &ps) {
            ps.push(x);
        }
        x += 2;
    }
    println!("{:?}", ps);
}


fn main() {
    prime(100);
    prime(500);
}
