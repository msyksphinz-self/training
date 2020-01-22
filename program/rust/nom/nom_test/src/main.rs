// use nom::branch::*;
use nom::bytes::complete::*;
// use nom::character::complete::*;
// use nom::combinator::*;
// use nom::multi::*;
use nom::sequence::*;
use nom::IResult;

pub fn parse_paren(s: &str) -> IResult<&str, &str> {
    delimited(tag("("),
              tag("test"),
              tag(")"),
              tag("end"),
    )(s)
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test() {
        assert_eq!(
            format!("{:?}", parse_paren("(test)end")),
            "Ok((\"\", \"test\"))",
        );
    }
}
