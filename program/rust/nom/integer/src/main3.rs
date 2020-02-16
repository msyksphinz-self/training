use nom::branch::*;
use nom::bytes::complete::*;
use nom::character::complete::*;
use nom::combinator::*;
use nom::multi::*;
use nom::sequence::*;
use nom::IResult;

#[derive(Debug)]
pub enum Number<'a> {
    IntegralNumber(IntegralNumber<'a>),
    RealNumber(RealNumber),
}

#[derive(Debug)]
pub enum IntegralNumber<'a> {
    DecimalNumber(DecimalNumber<'a>),
    OctalNumber(OctalNumber<'a>),
    BinaryNumber(BinaryNumber<'a>),
    HexNumber(HexNumber<'a>),
    UnsignedNumber(Vec<&'a str>),
}

#[derive(Debug)]
pub enum RealNumber {
    FixedPointNumber,
    FloatingPointNumber,
}

pub fn number(s: &str) -> IResult<&str, Number> {
    alt((integral_number, real_number))(s)
}

pub fn integral_number(s: &str) -> IResult<&str, Number> {
    let (s, x) = alt((
        octal_number,
        binary_number,
        hex_number,
        decimal_number,
        integral_unsigned_number,
    ))(s)?;
    Ok((s, Number::IntegralNumber(x)))
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

pub fn integral_unsigned_number(s: &str) -> IResult<&str, IntegralNumber> {
    let (s, x) = unsigned_number(s)?;
    Ok((s, IntegralNumber::UnsignedNumber(x)))
}

#[derive(Debug)]
pub struct BinaryNumber<'a> {
    pub size: Option<Vec<&'a str>>,
    pub binary_base: &'a str,
    pub binary_value: Vec<&'a str>,
}

pub fn binary_number(s: &str) -> IResult<&str, IntegralNumber> {
    let (s, (size, _, binary_base, _, binary_value)) =
        tuple((opt(size), space0, binary_base, space0, binary_value))(s)?;
    Ok((
        s,
        IntegralNumber::BinaryNumber(BinaryNumber {
            size,
            binary_base,
            binary_value,
        }),
    ))
}

#[derive(Debug)]
pub struct OctalNumber<'a> {
    pub size: Option<Vec<&'a str>>,
    pub octal_base: &'a str,
    pub octal_value: Vec<&'a str>,
}

pub fn octal_number(s: &str) -> IResult<&str, IntegralNumber> {
    let (s, (size, _, octal_base, _, octal_value)) =
        tuple((opt(size), space0, octal_base, space0, octal_value))(s)?;
    Ok((
        s,
        IntegralNumber::OctalNumber(OctalNumber {
            size,
            octal_base,
            octal_value,
        }),
    ))
}

#[derive(Debug)]
pub struct HexNumber<'a> {
    pub size: Option<Vec<&'a str>>,
    pub hex_base: &'a str,
    pub hex_value: Vec<&'a str>,
}

pub fn hex_number(s: &str) -> IResult<&str, IntegralNumber> {
    let (s, (size, _, hex_base, _, hex_value)) =
        tuple((opt(size), space0, hex_base, space0, hex_value))(s)?;
    Ok((
        s,
        IntegralNumber::HexNumber(HexNumber {
            size,
            hex_base,
            hex_value,
        }),
    ))
}

pub fn size(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = is_a("123456789")(s)?;
    let (s, x) = fold_many0(alt((tag("_"), digit1)), vec![x], |mut acc: Vec<_>, item| {
        acc.push(item);
        acc
    })(s)?;
    Ok((s, x))
}

pub fn real_number(s: &str) -> IResult<&str, Number> {
    let (s, x) = alt((fixed_point_number, floating_point_number))(s)?;
    Ok((s, Number::RealNumber(x)))
}

pub fn fixed_point_number(s: &str) -> IResult<&str, RealNumber> {
    Ok((s, RealNumber::FixedPointNumber))
}

pub fn floating_point_number(s: &str) -> IResult<&str, RealNumber> {
    Ok((s, RealNumber::FloatingPointNumber))
}

pub fn unsigned_number(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = digit1(s)?;
    fold_many0(alt((tag("_"), digit1)), vec![x], |mut acc: Vec<_>, item| {
        acc.push(item);
        acc
    })(s)
}

pub fn binary_value(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = is_a("01xXzZ?")(s)?;
    fold_many0(
        alt((tag("_"), is_a("01xXzZ?"))),
        vec![x],
        |mut acc: Vec<_>, item| {
            acc.push(item);
            acc
        },
    )(s)
}

pub fn octal_value(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = is_a("01234567xXzZ?")(s)?;
    fold_many0(
        alt((tag("_"), is_a("01234567xXzZ?"))),
        vec![x],
        |mut acc: Vec<_>, item| {
            acc.push(item);
            acc
        },
    )(s)
}

pub fn hex_value(s: &str) -> IResult<&str, Vec<&str>> {
    let (s, x) = is_a("0123456789abcdefABCDEFxXzZ?")(s)?;
    fold_many0(
        alt((tag("_"), is_a("0123456789abcdefABCDEFxXzZ?"))),
        vec![x],
        |mut acc: Vec<_>, item| {
            acc.push(item);
            acc
        },
    )(s)
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

pub fn binary_base(s: &str) -> IResult<&str, &str> {
    alt((
        tag("'b"),
        tag("'sb"),
        tag("'Sb"),
        tag("'B"),
        tag("'sB"),
        tag("'SB"),
    ))(s)
}

pub fn octal_base(s: &str) -> IResult<&str, &str> {
    alt((
        tag("'o"),
        tag("'so"),
        tag("'So"),
        tag("'O"),
        tag("'sO"),
        tag("'SO"),
    ))(s)
}

pub fn hex_base(s: &str) -> IResult<&str, &str> {
    alt((
        tag("'h"),
        tag("'sh"),
        tag("'Sh"),
        tag("'H"),
        tag("'sH"),
        tag("'SH"),
    ))(s)
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
    println!("result {:?}", decimal_number("'h100"));
}
