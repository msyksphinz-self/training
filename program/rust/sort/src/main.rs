// Random Generator
struct Rand {
    seed: u32
}

// Random Maximum Value
const RAND_MAX: u32 = 0xffff_ffff;

impl Rand {
    // Generator
    fn new(x: u32) -> Rand {
        Rand { seed: x }
    }

    fn rand(&mut self) -> u32 {
        let x = self.seed as u64;
        self.seed = ((69069 * x + 1) & RAND_MAX as u64) as u32;
        self.seed
    }
    
    fn random(&mut self) -> f64 {
        (1.0 / (RAND_MAX as f64 + 1.0)) * self.rand() as f64
    }

    // Shuffle arrays
    fn shuffle<T>(&mut self, buff: &mut [T]) {
        for i in 0 .. buff.len() {
            let j = (self.random() * buff.len() as f64) as usize;
            buff.swap(i, j);
        }
    }
}

// Bubble Sort
fn buble_sort<T: Ord>(buff: &mut [T]) {
    for i in 0..buff.len() {
        let mut j = buff.len() - 1;
        while j > i {
            if buff[j] < buff[j - 1] {
                buff.swap(j, j - 1);
            }
            j -= 1;
        }
    }
}


// Select Sort
fn select_sort<T: Ord> (buff: &mut [T])
{
    for i in 0..buff.len() {
        for j in i..buff.len() {
            if buff[i] > buff[j] {
                buff.swap(i, j);
            }
        }
    }
}


// Quick Sort
fn quick_sort<T: Ord + Copy> (buff: &mut [T])
{
    if buff.len() < 2 { return; }
    let pivot = buff[buff.len() / 2];
    let mut i = 0;
    let mut j = buff.len() - 1;
    loop {
        while pivot > buff[i] { i += 1; }
        while pivot < buff[j] { j -= 1; }
        if i >= j { break; }
        buff.swap(i, j);
        i += 1;
        j -= 1;
    }
    if i > 0 { quick_sort (&mut buff[.. i]); }
    if j < buff.len() -1 { quick_sort (&mut buff[j + 1 ..]); }
}


fn main() {
    let mut rng = Rand::new(1);

    let mut buff: [i32; 20] = [0; 20];
    for i in 0 .. 20 {
        buff[i] = i as i32;
    }
    rng.shuffle(&mut buff);
    println!("----- Before Sort ------");
    println!("{:?}", buff);
    select_sort(&mut buff);
    println!("----- Select Sort ------");
    println!("{:?}", buff);

    rng.shuffle(&mut buff);
    println!("----- Before Sort ------");
    println!("{:?}", buff);
    quick_sort(&mut buff);
    println!("----- Select Sort ------");
    println!("{:?}", buff);

}
