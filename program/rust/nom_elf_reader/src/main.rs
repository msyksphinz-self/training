use crate::nom;

fn parse_circuit(i: &[u8]) -> IResult<&[u8], &u8> {
    let(s, _) = tag()(s)?;

}

fn main() {
    let filename = args.bin_file[0].clone();
    let file = File::open(filename.clone()).unwrap();
    let filebuf = BufReader::new(file);


}
