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
                    let shdr_strtab = &elf.shdr_strtab;
                    for section in &elf.section_headers {
                        println!("elf.section_headers = {:#?}, file_offset = {:#x}, size = {:#x}",
                                 &shdr_strtab[section.sh_name],
                                 section.sh_offset,
                                 section.sh_size
                        );
                        for idx in 0..section.sh_size {
                            let offset = idx+section.sh_offset;
                            print!("{:02x}", buffer[offset as usize]);
                            if idx % 4 == 3 {
                                print!("\n");
                            }
                        }
                    }
                    let sym_strtab = &elf.strtab;
                    for symbol in &elf.syms {
                        println!("elf.symbol = {:#?} {:#x}",
                                 &sym_strtab[symbol.st_name], symbol.st_value);
                    }
                    println!("pe: {:#?}", &elf);
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
    match run() {
        Ok(()) => (),
        Err(err) => println!("{:#}", err)
    }
}

