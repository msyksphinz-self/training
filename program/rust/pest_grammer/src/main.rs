extern crate pest;
#[macro_use]
extern crate pest_derive;

use pest::Parser;

#[derive(Parser)]
#[grammar = "ident.pest"]
struct IdentParser;

#[test]
fn pos_int() {
    assert!(IdentParser::parse(Rule::pos_int, "0").is_err());
    assert!(IdentParser::parse(Rule::pos_int, "01").is_err());
    assert!(IdentParser::parse(Rule::pos_int, "012").is_err());
    assert!(IdentParser::parse(Rule::pos_int, "1").is_ok());
    assert!(IdentParser::parse(Rule::pos_int, "9").is_ok());
    assert!(IdentParser::parse(Rule::pos_int, "10").is_ok());
    assert!(IdentParser::parse(Rule::pos_int, "19").is_ok());
    assert!(IdentParser::parse(Rule::pos_int, "210").is_ok());
    assert!(IdentParser::parse(Rule::pos_int, "289").is_ok());

    assert!(IdentParser::parse(Rule::id_lit, "hello").is_ok());
    assert!(IdentParser::parse(Rule::id_lit, "world").is_ok());
    assert!(IdentParser::parse(Rule::id_lit, "010").is_err());
    assert!(IdentParser::parse(Rule::id_lit, "1234").is_err());
    assert!(IdentParser::parse(Rule::id_lit, "hello$world").is_ok());
    assert!(IdentParser::parse(Rule::id_lit, "1hello$world").is_err());

    assert!(IdentParser::parse(Rule::keyword_as_id, "circuit").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "module").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "extmodule").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "parameter").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "input").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "output").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "UInt").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "SInt").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "Clock").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "Analog").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "Fixed").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "flip").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "wire").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "reg").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "with").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "reset").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "mem").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "depth").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "reader").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "writer").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "readwriter").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "inst").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "of").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "node").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "is").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "invalid").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "when").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "else").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "stop").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "printf").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "skip").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "old").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "new").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "undefined").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "mux").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "validif").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "cmem").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "smem").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "mport").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "infer").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "read").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "write").is_ok());
    assert!(IdentParser::parse(Rule::keyword_as_id, "rdwr").is_ok());
}
