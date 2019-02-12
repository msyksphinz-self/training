fn factorization (n: i32)
{
    fn factor_sub(n: i32, m: i32) -> (i32, i32) {
        let mut c = 0;
        let mut x = n;
        while x % m == 0 {
            c += 1;
            x /= m;
        }
        (c, x)
    }

    let (c, n) = factor_sub(n, 2);
    if c > 0 {
        print!("({},{})", 2, c);
    }
    let mut x = 3;
    let mut m = n;
    while x * x <= m {
        let (c, n) = factor_sub(m, x);
        if c > 0 {
            print!("({},{})", x, c);
        }
        m = n;
        x += 2;
    }
    if m > 1 {
        print!("({},{})", m, 1);
    }
    println!("");
}


fn main() {
    factorization(24);
    factorization(12345678);
    factorization(123456789);
    factorization(1234567890);
    factorization(1111111111);
}
