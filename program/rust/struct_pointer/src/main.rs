struct Point<'a, 'b> {
    x: i32,
    y: i32,
    func: &'a Option<&'a PointFunc<'a, 'b>>,
}

impl <'a, 'b>Point<'a, 'b> {
    fn new(x: i32, y: i32) -> Point<'a, 'b> {
        Point {
            x, y, func:&None
        }
    }
}


struct PointFunc<'a, 'b> {
    ptr: &'a Point<'a, 'b>,
}

impl <'a, 'b>PointFunc<'a, 'b> {

    fn newPoint(&mut self, data: &'a Point) -> &'b Box<Point> {
        let new_point = Box::new(Point{x: data.x + data.y, y: 0, func:&None});
        return new_point;
    }

    fn call(&mut self, data: &'a Point) {
        self.ptr = data;
    }
    fn forward(&mut self) -> i32 {
        return self.ptr.x + self.ptr.y
    }
}


fn main() {
    let p = Box::new(Point{x:10, y:20, func:&None});
    let mut pp = PointFunc{ptr: &p};

    let new_p = pp.newPoint(&p);

    let f = pp.forward();

    println!("x, y = {}, {}", pp.ptr.x, pp.ptr.y);
    println!("x, y = {}, {}", new_p.x, new_p.y);
    println!("x + y = {}", f);
}
