use std::ops::Add;
use num::cast::NumCast;

struct SampleStruct<T>
{
    m_x: T,
    m_y: T
}


impl SampleStruct<i64>
{
    pub fn new64() -> SampleStruct<i64>
    {
        SampleStruct::<i64> {
            m_x: 0,
            m_y: 0,
        }
    }
//    pub fn incr(&mut self) {
//        self.m_x += 1;
//        self.m_y += 1;
//    }
}


impl SampleStruct<i32>
{
    pub fn new32() -> SampleStruct<i32>
    {
        SampleStruct::<i32> {
            m_x: 0,
            m_y: 0,
        }
    }
//    pub fn incr(&mut self) {
//        self.m_x += 1;
//        self.m_y += 1;
//    }
}

impl<T: Copy + Add<T, Output=T>> SampleStruct<T>
where T:num::cast::NumCast
{
    pub fn incr(&mut self) {
        self.m_x = self.m_x + NumCast::from(1).unwrap();
        self.m_y = self.m_y + NumCast::from(1).unwrap();
    }
}



fn main() {
    let mut sample_struct32 = SampleStruct::new32();
    println!(" sample_struct32.x = {:08x}", sample_struct32.m_x);
    println!(" sample_struct32.y = {:08x}", sample_struct32.m_y);
    sample_struct32.incr();
    println!(" sample_struct32.x = {:08x}", sample_struct32.m_x);
    println!(" sample_struct32.y = {:08x}", sample_struct32.m_y);

    let mut sample_struct64 = SampleStruct::new64();
    println!(" sample_struct64.x = {:08x}", sample_struct64.m_x);
    println!(" sample_struct64.y = {:08x}", sample_struct64.m_y);
    sample_struct64.incr();
    println!(" sample_struct64.x = {:08x}", sample_struct64.m_x);
    println!(" sample_struct64.y = {:08x}", sample_struct64.m_y);
}
