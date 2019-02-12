const ESIZE: usize = 9;
const MAXS: usize = 9 * 8 * 7 * 6 * 5 * 4 * 3;

// Stages
struct State {
    board: [i32; ESIZE],
    space: usize,
    prev: usize
}


fn position(x: i32, xs: &[i32]) -> usize {
    for i in 0 .. ESIZE + 1 {
        if xs[i] == x { return i; }
    }
    ESIZE
}


fn print_answer (xs: &Vec<State>, x: usize) {
    if xs[x].prev != MAXS {
        print_answer (xs, xs[x].prev);
    }
    println!("{:?}", xs[x].board);
}

use std::collections::HashSet;

fn eight_bfs (start: &[i32; ESIZE], goal: &[i32; ESIZE], adj: &[Vec<usize>])
{
    let mut que: Vec<State> = Vec::with_capacity(MAXS);
    let mut chk: HashSet<[i32; ESIZE]> = HashSet::new();
    let mut rp = 0;
    que.push(State { board: *start,
                     space: position(0, start),
                     prev:  MAXS });
    chk.insert(*start);
    while rp < que.len () {
        let s = que[rp].space;
        for x in &adj[s] {
            let mut new_board = que[rp].board;
            new_board[s] = new_board[*x];
            new_board[*x] = 0;
            if !chk.contains(&new_board) {
                chk.insert(new_board);
                que.push(State {board: new_board, space: *x, prev: rp });
                if new_board == *goal {
                    print_answer (&que, que.len() - 1);
                    return;
                }
            }
        }
        rp += 1;
    }
}

fn main() {
    let adj8: [Vec<usize>; ESIZE] = [
        vec![1, 3],
        vec![0, 2, 4],
        vec![1, 5],
        vec![0, 4, 6],
        vec![1, 3, 5, 7],
        vec![2, 4, 8],
        vec![3, 7],
        vec![4, 6, 8],
        vec![5, 7]
    ];
    println!("----- eight_bfs -----");
    eight_bfs(&[8, 6, 7, 2, 5, 4, 3, 0, 1], &[1, 2, 3, 4, 5, 6, 7, 8, 0], &adj8);
}

