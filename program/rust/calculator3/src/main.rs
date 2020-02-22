#[macro_use] extern crate lalrpop_util;

lalrpop_mod!(pub calculator3); // synthesized by LALRPOP

#[test]
fn calculator3() {
    let v = calculator3::ExprParser::new().parse("((22 * 44) + 66)").unwrap();
    assert_eq!(v, 1034);
}
