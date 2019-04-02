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


fn main() {
}


