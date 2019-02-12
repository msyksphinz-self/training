const BSIZE: usize = 9;

fn print_sudoku (board: &[[i32; BSIZE]; BSIZE]) {
    for x in 0 .. BSIZE {
        for y in 0 .. BSIZE {
            print!("{} ", board[x][y]);
        }
        println!("");
    }
}


fn check_sudoku (x: usize, y: usize, n: i32, board: &[[i32; BSIZE]; BSIZE]) -> bool {
    for i in 0 .. BSIZE {
        if board[i][y] == n || board[x][i] == n {
            return false;
        }
    }
    let x1 = (x / 3) * 3;
    let y1 = (y / 3) * 3;
    for i in 0 .. 3 {
        for j in 0 .. 3 {
            if board[x1 + i][y1 + j] == n {
                return false;
            }
        }
    }
    true
}


// Solve Number Place
fn sudoku (x: usize, y: usize, board: &mut[[i32; BSIZE]; BSIZE])
{
    if y >= BSIZE {
        print_sudoku (board);
    } else if x >= BSIZE {
        sudoku (0, y + 1, board);
    } else if board[x][y] != 0 {
        sudoku (x + 1, y, board);
    } else {
        for n in 1 .. 10 {
            if !check_sudoku (x, y, n, board) { continue; }
            board[x][y] = n;
            sudoku(x + 1, y, board);
            board[x][y] = n;
            sudoku(x + 1, y, board);
            board[x][y] = 0;
        }
    }
}

fn main() {
    let mut sudoku_board = [
        [5, 3, 0,  0, 7, 0,  0, 0, 0],
        [6, 0, 0,  1, 9, 5,  0, 0, 0],
        [0, 9, 8,  0, 0, 0,  0, 6, 0],

        [8, 0, 0,  0, 6, 0,  0, 0, 3],
        [4, 0, 0,  8, 0, 3,  0, 0, 1],
        [7, 0, 0,  0, 2, 0,  0, 0, 6],

        [0, 6, 0,  0, 0, 0,  2, 8, 0],
        [0, 0, 0,  4, 1, 9,  0, 0, 5],
        [0, 0, 0,  0, 8, 0,  0, 7, 9],
    ];
    print_sudoku(&sudoku_board);
    println!("--------------------");
    sudoku(0, 0, &mut sudoku_board);
}
