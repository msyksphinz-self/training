fn main() {
    let arithmetic = grammar! {
        // a grammar can have as many non-terminals as you want, and can return any type
        parse: i64 = <term> "="

        // alternatives are separated by `|`,
        // and the `=> { ... }` syntax is used to manipulate the output of the parser before returning it
            term: i64 = <l: factor> "+" <r: term> => { l + r }
        | <l: factor> "-" <r: term> => { l - r }
        | factor

        // the `<...>` syntax is used to capture the output of a sub-parser,
        // and optionally assign it to a local variable with `<name: ...>`
            factor: i64 = <l: value> "*" <r: factor> => { l * r }
        | <l: value> "/" <r: factor> => { l / r }
        | value

            value: i64 = ("0"|"1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9")+ => { result.join("").parse::<i64>().unwrap() }
        | "(" <term> ")"
    };

    // when the grammar is defined you can use any of the non-terminals as parser functions
    assert_eq!(arithmetic.parse("123="), Ok(("", 123 as i64)));
    assert_eq!(arithmetic.parse("1+1="), Ok(("", 2 as i64)));
    assert_eq!(arithmetic.parse("12+(3*7)="), Ok(("", 33 as i64)));
}
