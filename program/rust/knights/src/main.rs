const BSIZE: usize = 9;

// Move
static DX: [i32; 8] = [ 1,  2, 2, 1, -1, -2, -2, -1];
static DY: [i32; 8] = [-2, -1, 1, 2,  2,  1, -1, -2];

// Initialize Board
fn init_board (board: &mut[[i32; BSIZE]; BSIZE])
{
    for x in 2 .. 7 {
        for y in 2 .. 7 {
            board[x][y] = 0;
        }
    }
}

// Print Board
fn print_board (board: &[[i32; BSIZE]; BSIZE]) {
    for x in 2 .. 7 {
        for y in 2 .. 7 {
            print!("{:>2} ", board[x][y]);
        }
        println!("");
    }
    println!("");
}


// Solve Knights
fn knight_tour (n: i32, x: i32, y: i32, board: &mut[[i32; BSIZE]; BSIZE])
{
    if n > 25 {
        print_board(board);
    } else {
        for i in 0 .. DX.len() {
            let x1 = x + DX[i];
            let y1 = y + DY[i];
            if board[x1 as usize][y1 as usize] == 0 {
                board[x1 as usize][y1 as usize] = n;
                knight_tour (n + 1, x1, y1, board);
                board[x1 as usize][y1 as usize] = 0;
            }
        }
    }
}

fn main () {
    let mut board = [[-1; BSIZE]; BSIZE];
    init_board(&mut board);
    board[2][2] = 1;
    knight_tour(2, 2, 2, &mut board);
}


    
                    
