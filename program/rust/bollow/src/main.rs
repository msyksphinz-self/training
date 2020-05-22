struct Point {
    x: i32,
    y: i32,
}

impl Point {

    fn newPoint(data: &Point) -> Box<Point> {
        let new_point = Box::new(Point{x: data.x + data.y, y: 0});
        return new_point;
    }

    fn calc(&mut self, val: i32) -> i32 {
        return self.x + self.y + val
    }
}


fn main() {
    println!("Hello, world!");

    let mut p = Point{x:10, y:20};
    let mut pp = Point::newPoint(&p);

    let data1 = pp.calc(100);
    let data2 = pp.calc(200);

    println!("data1 = {}", data1);
    println!("data2 = {}", data2);
}
