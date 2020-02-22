#[macro_use] extern crate lalrpop_util;

lalrpop_mod!(pub decimal_lalr); // synthesized by LALRPOP

fn main() {
    println!("Hello, world!");
}


#[test]
fn decimal_lalr_test() {
    assert!(decimal_lalr::PosIntParser::new().parse("0").is_err());
    assert!(decimal_lalr::PosIntParser::new().parse("1").is_ok());
    assert!(decimal_lalr::PosIntParser::new().parse("10").is_ok());
    assert!(decimal_lalr::DigitParser::new().parse("0").is_ok());
    assert!(decimal_lalr::DigitParser::new().parse("1").is_ok());
    assert!(decimal_lalr::DigitParser::new().parse("10").is_ok());
}
