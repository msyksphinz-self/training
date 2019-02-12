// select r numbers from 1 to n

fn combinations(n: i32, r: i32, func: fn(&Vec<i32>) -> ()) {
    fn comb(n: i32, m: i32, r: i32, func: fn(&Vec<i32>) -> (), xs: &mut Vec<i32>) {
        if r == 0 {
            func(xs);
        } else if m <= n {
            xs.push(m);
            comb(n, m + 1, r - 1, func, xs);
            xs.pop();
            comb(n, m + 1, r, func, xs);
        }
    }
    comb(n, 1, r, func, &mut vec![]);
}

fn print_vector(xs: &Vec<i32>) {
    println!("{:?}", xs);
}


fn main() {
    combinations(5, 3, print_vector);
}
