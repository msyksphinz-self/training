// Komachi calculation

const ADD: i32 = -1;
const SUB: i32 = -2;

fn calc_expr (expr: &Vec<i32>) -> i32 {
    let mut acc = expr[0];
    let mut x = 1;
    while x < expr.len() {
        if expr[x] == ADD {
            acc += expr[x + 1];
        } else {
            acc -= expr[x + 1];
        }
        x += 2;
    }
    acc
}

fn print_expr (xs: &Vec<i32>) {
    for x in xs {
        match *x {
            ADD => print!(" + "),
            SUB => print!(" - "),
            _ => print!("{}", x)
        }
    }
    println!(" = 100");
}

fn komachi(n: i32, expr: &mut Vec<i32>) {
    if n == 10 {
        if calc_expr(expr) == 100 {
            print_expr(expr);
        }
    } else {
        let idx = expr.len();
        expr.push(ADD);
        expr.push(n);
        komachi (n + 1, expr);
        expr[idx] = SUB;
        komachi (n + 1, expr);
        expr.pop();
        expr.pop();
        let save = expr[idx - 1];
        expr[idx - 1] = expr[idx - 1] * 10 + n;
        komachi (n + 1, expr);
        expr[idx - 1] = save;
    }
}


fn main() {
    komachi (2, &mut vec![1]);
}
