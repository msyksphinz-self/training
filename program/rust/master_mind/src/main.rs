// Generate Permutations
fn permutations(n: i32, m: i32) -> Vec<Vec<i32>>
{
    fn perm(n: i32, m: i32, xs: &mut Vec<i32>, ps: &mut Vec<Vec<i32>>)
    {
        if m == 0 {
            ps.push(xs.clone());
        } else {
            for x in 0 .. n + 1 {
                if xs.contains(&x) { continue; }
                xs.push(x);
                perm(n, m-1, xs, ps);
                xs.pop();
            }
        }
    }

    let mut ps: Vec<Vec<i32>> = vec![];
    perm (n, m, &mut vec![], &mut ps);
    ps
}


// Count Bulls
fn count_bulls (xs: &Vec<i32>, ys: &Vec<i32>) -> i32 {
    let mut c = 0;
    for i in 0 .. xs.len() {
        if xs[i] == ys[i] { c += 1; }
    }
    c
}

// Count Same Number
fn count_same_number (xs: &Vec<i32>, ys: &Vec<i32>) -> i32 {
    let mut c = 0;
    for x in xs {
        if ys.contains(x) {
            c += 1;
        }
    }
    c
}

struct Query {
    code: Vec<i32>, bulls: i32, cows: i32
}


fn check_query(code: &Vec<i32>, query_table: &[Query]) -> bool {
    for q in query_table {
        let b = count_bulls(&(q.code), code);
        let c = count_same_number(&(q.code), code) - b;
        if b != q.bulls || c != q.cows {
            return false;
        }
    }
    true
}


fn master_mind (answer: &Vec<i32>, code_table: &Vec<Vec<i32>>) {
    let mut qs: Vec<Query> = vec![];
    let mut n = 1;
    for ref code in code_table {
        if check_query(code, &qs) {
            let b = count_bulls (code, answer);
            let c = count_same_number (code, answer) - b;
            qs.push (Query {code: (*code).clone(), bulls: b, cows: c});
            println!("{}: {:?}, bulls = {}, cows = {}", n, code, b, c);
            n += 1;
            if b == 4 {
                println! ("Good Job!!!");
                return;
            }
        }
    }
    println!("Oops!, maybe bug!!");
}


fn main() {
    let code_table = permutations(9, 4);
    master_mind(&vec![9, 8, 7, 6], &code_table);
    master_mind(&vec![9, 4, 3, 1], &code_table);
}
