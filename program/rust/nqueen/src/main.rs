// Detect attack

fn attack (q: i32, qs: &Vec<i32>) -> bool {
    let mut i = qs.len();
    let mut d = 1;
    while i > 0 {
        if q + d == qs[i - 1] || q - d == qs[i - 1] { return true; }
        i -= 1;
        d += 1;
    }
    false
}

// Solve of N-Queen Problem
fn nqueens (n: i32) {
    fn queen_sub (n: i32, qs: &mut Vec<i32>) {
        if n == qs.len() as i32 {
            println!("{:?}", qs);
        } else {
            for q in 1 .. n+1 {
                if !qs.contains(&q) && !attack(q, qs) {
                    qs.push(q);
                    queen_sub(n, qs);
                    qs.pop();
                }
            }
        }
    }
    queen_sub(n, &mut vec![]);
}


fn main() {
    for n in 4 .. 9 {
        nqueens(n);
    }
}
