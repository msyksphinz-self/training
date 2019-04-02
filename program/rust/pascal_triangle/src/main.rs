// Pascal's triangle

// 2D array version
fn pascal2d() {
    const N: usize = 16;

    let mut table = [[1; N]; N];
    for i in 2..N {
        for j in 1 .. i {
            table[i][j] = table[i - 1][j - 1] + table[i - 1][j];
        }
    }
    for i in 0 .. N {
        for j in 0 .. i + 1 {
            print!("{} ", table[i][j]);
        }
        print!("");
    }
}


// 1D array version
fn pascal1d() {
    const N: usize = 20;
    let mut table = [1; N];
    println!("{}", table[0]);
    println!("{} {}", table[0], table[1]);
    for i in 2 .. N {
        let mut j = i - 1;
        while j > 0 {
            table[j] += table[j-1];
            j -= 1;
        }
        // Display
        for k in 0 .. i + 1 {
            print!("{} ", table[k]);
        }
        println!("");
    }
}
            

fn main() {
    pascal2d();
    pascal1d();
}
