use nom::branch::*;
use nom::bytes::complete::*;
use nom::character::complete::*;
use nom::combinator::*;
use nom::multi::*;
use nom::IResult;


// fragment
// PosInt
//   : [1-9] ( Digit )*
//   ;
fn parse_pos_int(s: &str) -> IResult<&str, &str> {
    let (s, x) = alt((tag("1"), tag("2"), tag("3"), tag("4"), tag("5"),
                      tag("6"), tag("7"), tag("8"), tag("9")))(s)?;
    let (s, x) = fold_many0(digit1, x, |acc: &str, item| {
        str_concat::concat(acc, item).unwrap()
    })(s)?;
    Ok((s, x))
}


// intLit
//   : UnsignedInt
//   | SignedInt
//   | HexLit
//   ;
fn parse_int_lit(s: &str) -> IResult<&str, &str> {
    alt((parse_unsigned_int_lit,
         parse_signed_int_lit,
         parse_hex_lit))(s)
}

// UnsignedInt
//   : '0'
//   | PosInt
//   ;
fn parse_unsigned_int_lit(s: &str) -> IResult<&str, &str> {
    alt((tag("0"),
         parse_pos_int))(s)
}

// SignedInt
//   : ( '+' | '-' ) PosInt
//   ;
fn parse_signed_int_lit(s: &str) -> IResult<&str, &str> {
    let (s, _pm) = alt((tag("+"), tag("-")))(s)?;
    parse_pos_int(s)
}


fn parse_hex_lit(s: &str) -> IResult<&str, &str> {
    let (s, _quote1) = tag("\"")(s)?;
    let (s, _h) = tag("h")(s)?;
    let (s, _pm) = opt(alt((tag("+"), tag("-"))))(s)?;
    let (s, a) = is_a("0123456789abcdfABCDF")(s)?;
    let (s, hex) = fold_many1(hex_digit1, a, |acc: &str, item| {
        str_concat::concat(acc, item).unwrap()
    })(s)?;
    let (s, _quote2) = tag("\"")(s)?;
    Ok((s, hex))
}


// StringLit
//   : '"' UnquotedString? '"'
//   ;
fn parse_string_lit(s: &str) -> IResult<&str, &str> {
    let (s, _x) = tag("\"")(s)?;
    let (s, str_val) = opt(parse_unquoted_string)(s)?;
    let (s, _x) = tag("\"")(s)?;
    let str_ret = if let Some(s) = str_val {
        s
    } else {
        ""
    };
    Ok((s, str_ret))
}


// RawString
//   : '\'' UnquotedString? '\''
//   ;
fn parse_raw_string(s: &str) -> IResult<&str, &str> {
    let (s, _x) = tag("\'")(s)?;
    let (s, x) = parse_unquoted_string(s)?;
    let (s, _x) = tag("\'")(s)?;
    Ok((s, x))
}


// fragment
// UnquotedString
//   : ( '\\\'' | '\\"' | ~[\r\n] )+?
//   ;
fn parse_unquoted_string(s: &str) -> IResult<&str, &str> {
    fn parse_unquoted_string_internal(s: &str) -> IResult<&str, &str> {
        alt((tag("\\\'"),
             tag("\\\""),
             is_not("\r\n")))(s)
    }
    let (s, a) = many1(parse_unquoted_string_internal)(s)?;
    let mut ret = None;
    for x in a {
        ret = if let Some(ret) = ret {
            Some(str_concat::concat(ret, x).unwrap())
        } else {
            Some(x)
        };
    }
    let ret_val = if let Some(b) = ret {
        b
    } else {
        ""
    };
    Ok((s, ret_val))
}


// Expr
//   : Expr '+' Expr
//   ; Term
// -->
// Expr
//   : Expr1 Term
// Expr1:
//   : e
//   : '+' Expr Expr1

fn parse_expr(s: &str) -> IResult<&str, String> {
    let (s, x) = parse_expr1(s)?;
    let (s, y) = parse_term(s)?;
    let ret = format!("{}{}", x, y);
    Ok((s, ret))
    // alt((parse_expr_plus_expr, parse_term))(s)
}

fn parse_expr1(s: &str) -> IResult<&str, String> {
    println!("parse_expr1 = {}", s);
    fn parse_plus_expr1 (s: &str) -> IResult<&str, String> {
        let (s, x) = char('+')(s)?;
        let (s, y) = parse_expr(s)?;
        let (s, z) = parse_expr1(s)?;
        // let ret = format!("{}{}{}", x, y, z);
        Ok((s, ret))
    }
    let (s, x) = opt(parse_plus_expr1)(s)?;
    let str_ret = if let Some(s) = x {
        s.to_string()
    } else {
        "".to_string()
    };
    Ok((s, str_ret))
}


fn parse_expr_plus_expr(s: &str) -> IResult<&str, String> {
    let (s, x) = parse_expr(s)?;
    let (s, y) = char('+')(s)?;
    let (s, z) = parse_expr(s)?;
    let ret = format!("{}{}{}", x, y, z);
    Ok((s, ret))
}

fn parse_term(s: &str) -> IResult<&str, String> {
    let (s, x) = char('1')(s)?;
    Ok((s, x.to_string()))
}



#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_int() {
        assert_eq!(
            format!("{:?}", parse_int_lit("1234567890")),
            "Ok((\"\", \"1234567890\"))",
        );
        assert_eq!(
            format!("{:?}", parse_int_lit("+1234567890")),
            "Ok((\"\", \"1234567890\"))",
        );
        assert_eq!(
            format!("{:?}", parse_int_lit("-1234567890")),
            "Ok((\"\", \"1234567890\"))",
        );

        assert_eq!(
            format!("{:?}", parse_int_lit("1234567890")),
            "Ok((\"\", \"1234567890\"))",
        );
        assert_eq!(
            format!("{:?}", parse_int_lit("+1234567890")),
            "Ok((\"\", \"1234567890\"))",
        );

        assert_eq!(
            format!("{:?}", parse_int_lit("\"h+ABCDEF01\"")),
            "Ok((\"\", \"ABCDEF01\"))",
        );
        assert_eq!(
            format!("{:?}", parse_int_lit("\"h-ABCDEF01\"")),
            "Ok((\"\", \"ABCDEF01\"))",
        );
        assert_eq!(
            format!("{:?}", parse_int_lit("\"hABCDEF01\"")),
            "Ok((\"\", \"ABCDEF01\"))",
        );
    }

    #[test]
    fn test_string() {
        assert_eq!(
            format!("{:?}", parse_string_lit("\"Hello World\"")),
            "Ok((\"\", \"Helo World\"))",
        );
    }

    #[test]
    fn test_expr() {
        assert_eq!(
            format!("{:?}", parse_expr("1+1")),
            "Ok((\"\", \"1+1\"))",
        );
    }
}


fn main() {
    // println!("result {:?}", parse_string_lit("\"Hello World\""))
    // println!("result {:?}", parse_unquoted_string("Hello World Hello World2"))

    println!("result {:?}", parse_expr("1+1"));
}
