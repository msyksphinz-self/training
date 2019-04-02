// Solve with Neighbor
const SIZE: usize = 7;

static MAT: [[bool; SIZE]; SIZE] = [
    // A      B      C      D      E      F      G
    [false, true,  true,  false, false, false, false],  // A
    [true,  false, true,  true,  false, false, false],  // B
    [true,  true,  false, false, true,  false, false],  // C
    [false, true , false, false, true,  true,  false],  // D
    [false, false, true,  true,  false, false, true ],  // E
    [false, false, false, true,  false, false, false],  // F
    [false, false, false, false, true,  false, false],  // G
];


// Depth First search
fn dfs(goal: usize, path: &mut Vec<usize>) {
    let x = path[path.len() - 1];
    if x == goal {
        println!("{:?}", path);
    } else {
        for i in 0 .. SIZE {
            if !MAT[x][i] || path.contains(&i) { continue; }
            path.push(i);
            dfs(goal, path);
            path.pop();
        }
    }
}


fn make_path (path: &Vec<usize>, x: usize) -> Vec<usize> {
    let mut xs = vec![];
    for &a in path {
        xs.push(a);
    }
    xs.push(x);
    xs
}

// Width First Search
fn bfs (start: usize, goal: usize) {
    let mut que = vec![vec![start]];
    while que.len() > 0 {
        let path = que.remove(0);
        let x = path[path.len() - 1];
        if x == goal {
            println!("{:?}", path);
        } else {
            for i in 0..SIZE {
                if !MAT[x][i] || path.contains(&i) { continue; }
                que.push(make_path(&path, i));
            }
        }
    }
}


fn ids(start: usize, goal: usize)
{
    fn dfs (limit: usize, goal: usize, path: &mut Vec<usize>) {
        let x = path[path.len() - 1];
        if limit == path.len() {
            if x == goal {
                println!("{:?}", path);
            }
        } else {
            for i in 0..SIZE {
                if !MAT[x][i] || path.contains(&i) { continue; }
                path.push(i);
                dfs(limit, goal, path);
                path.pop();
            }
        }
    }
    for limit in 2..SIZE + 1 {
        println!("----- {} -----", limit);
        dfs(limit, goal, &mut vec![start]);
    }
}

//
// 隣接リストによる解法
//

// 深さ優先探索
fn dfs_adj(goal: usize, path: &mut Vec<usize>, adj: &[Vec<usize>]) {
    let x = path[path.len() - 1];
    if x == goal {
        println!("{:?}", path);
    } else {
        for y in &adj[x] {
            if path.contains(y) { continue; }
            path.push(*y);
            dfs_adj(goal, path, adj);
            path.pop();
        }
    }
}

// 幅優先探索
fn bfs_adj(start: usize, goal: usize, adj: &[Vec<usize>]) {
    let mut que = vec![vec![start]];
    while que.len() > 0 {
        let path = que.remove(0);
        let x = path[path.len() - 1];
        if x == goal {
            println!("{:?}", path);
        } else {
            for y in &adj[x] {
                if path.contains(y) { continue; }
                que.push(make_path(&path, *y));
            }
        }
    }
}

// 反復深化
fn ids_adj(start: usize, goal: usize, adj: &[Vec<usize>]) {
    fn dfs(limit: usize, goal: usize, path: &mut Vec<usize>, adj: &[Vec<usize>]) {
        let x = path[path.len() - 1];
        if limit == path.len() {
            if x == goal {
                println!("{:?}", path);
            }
        } else {
            for y in &adj[x] {
                if path.contains(y) { continue; }
                path.push(*y);
                dfs(limit, goal, path, adj);
                path.pop();
            }
        }
    }
    for limit in 2 .. SIZE + 1 {
        println!("----- {} -----", limit);
        dfs(limit, goal, &mut vec![start], adj);
    }
}

fn main() {
    println!("----- dfs -----");
    dfs(6, &mut vec![0]);
    println!("----- bfs -----");
    bfs(0, 6);
    println!("----- ids -----");
    ids(0, 6);

    // 隣接リスト Vec は static に配置できないようだ
    let adj: [Vec<usize>; SIZE] = [
        vec![1, 2],
        vec![0, 2, 3],
        vec![0, 1, 4],
        vec![1, 4, 5],
        vec![2, 3, 6],
        vec![3],
        vec![4]
    ];
    println!("----- dfs_adj -----");
    dfs_adj(6, &mut vec![0], &adj);
    println!("----- bfs_adj -----");
    bfs_adj(6, 0, &adj);
    println!("----- ids_adj -----");
    ids_adj(6, 0, &adj);
}
