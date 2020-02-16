use nom::branch::*;
use nom::bytes::complete::*;
use nom::character::complete::*;
use nom::combinator::*;
use nom::multi::*;
use nom::sequence::*;
use nom::IResult;


#[derive(Debug)]
pub enum IntegralNumber<'a> {
    DecimalNumber(DecimalNumber<'a>),
}

pub fn decimal_base(s: &str) -> IResult<&str, &str> {
    alt((
        tag("'d"),
        tag("'sd"),
        tag("'Sd"),
        tag("'D"),
        tag("'sD"),
        tag("'SD"),
    ))(s)
}

pub fn size(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = is_a("123456789")(s)?;
    let (s, x) = fold_many0(alt((tag("_"), digit1)), vec![x], |mut acc: Vec<_>, item| {
        acc.push(item);
        acc
    })(s)?;
    Ok((s, x))
}


#[derive(Debug)]
pub struct DecimalNumber<'a> {
    pub size: Option<Vec<&'a str>>,
    pub decimal_base: &'a str,
    pub decimal_number: Vec<&'a str>,
}

pub fn decimal_number(s: &str) -> IResult<&str, IntegralNumber> {
    let (s, (size, _, decimal_base, _, decimal_number)) = tuple((
        opt(size),
        space0,
        decimal_base,
        space0,
        alt((unsigned_number, x_number, z_number)),
    ))(s)?;
    Ok((
        s,
        IntegralNumber::DecimalNumber(DecimalNumber {
            size,
            decimal_base,
            decimal_number,
        }),
    ))
}


pub fn unsigned_number(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = digit1(s)?;
    fold_many0(alt((tag("_"), digit1)), vec![x], |mut acc: Vec<_>, item| {
        acc.push(item);
        acc
    })(s)
}

pub fn x_number(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = alt((tag("x"), tag("X")))(s)?;
    fold_many0(
        alt((tag("_"), is_a("_"))),
        vec![x],
        |mut acc: Vec<_>, item| {
            acc.push(item);
            acc
        },
    )(s)
}

pub fn z_number(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = alt((tag("z"), tag("Z"), tag("?")))(s)?;
    fold_many0(
        alt((tag("_"), is_a("_"))),
        vec![x],
        |mut acc: Vec<_>, item| {
            acc.push(item);
            acc
        },
    )(s)
}

fn main() {
    // println!("result {:?}", decimal_number("'d100"));
    println!("result {:?}", size("100"));
}
