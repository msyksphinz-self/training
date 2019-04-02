// Select m numbers from 1 into n

fn permutations(n: i32, m: i32) {
    fn perm(n: i32, m: i32, xs: &mut Vec<i32>) {
        if m == 0 {
            print_vector(xs);
        } else {
            for x in 1 .. n + 1 {
                if !xs.contains(&x) {
                    xs.push(x);
                    perm(n, m - 1, xs);
                    xs.pop();
                }
            }
        }
    }
    perm(n, m, &mut vec![]);
}

// Print Vector
fn print_vector(xs: &Vec<i32>) {
    println!("{:?}", xs);
}


fn main() {
    permutations(4, 4);
}
