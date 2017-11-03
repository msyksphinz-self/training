extern crate goblin;
use goblin::{error, Object};
use std::path::Path;
use std::env;
use std::fs::File;
use std::io::Read;

fn run () -> error::Result<()> {
    for (i, arg) in env::args().enumerate() {
        if i == 1 {
            let path = Path::new(arg.as_str());
            let mut fd = File::open(path)?;
            let mut buffer = Vec::new();
            fd.read_to_end(&mut buffer)?;
            match Object::parse(&buffer)? {
                Object::Elf(elf) => {
                    // println!("elf: {:#?}", &elf);
                    for header in &elf.program_headers {
                        println!("elf.program_headers = {:#?}", header);
                    }
                    let shdr_strtab = &elf.shdr_strtab;
                    for section in &elf.section_headers {
                        println!("elf.section_headers = {:#?}", &shdr_strtab[section.sh_name]);
                    }
                    let sym_strtab = &elf.strtab;
                    for symbol in &elf.syms {
                        println!("elf.symbol = {:#?} {:#x}", &sym_strtab[symbol.st_name], symbol.st_value);
                    }
                }
                Object::PE(pe) => {
                    println!("pe: {:#?}", &pe);
                },
                Object::Mach(mach) => {
                    println!("mach: {:#?}", &mach);
                },
                Object::Archive(archive) => {
                    println!("archive: {:#?}", &archive);
                },
                Object::Unknown(magic) => { println!("unknown magic: {:#x}", magic) }
            }
        }
    }
    Ok(())
}

fn main () {
    run ();
}

