extern crate goblin;
use goblin::{error, Object};
use std::collections::HashMap;
use std::path::Path;
use std::env;
use std::fs::File;
use std::io::Read;

fn memory_elf (elf_obj: goblin::elf::Elf,
               memory: &mut std::collections::HashMap<u64, u8>,
               buffer: &std::vec::Vec<u8>) -> error::Result<()>
{
    let shdr_strtab = &elf_obj.shdr_strtab;
    for section in &elf_obj.section_headers {
        println!("elf_obj.section_headers = {:#?}, file_offset = {:#x}, size = {:#x}",
                 &shdr_strtab[section.sh_name],
                 section.sh_offset,
                 section.sh_size
        );
        if section.sh_size != 0 {
            for idx in 0..(section.sh_size-1) {
                let mut offset = idx + section.sh_offset;
                memory.insert(section.sh_addr + idx, buffer[offset as usize]);
            }
        }
    }
    Ok(())
}


fn dump_memory (memory: &std::collections::HashMap<u64, u8>)
{
    for (addr, data) in memory.iter() {
        println!("{:#x}: {:#02x}", addr, data);
    }
}


fn load_elf (hexfile: String) -> error::Result<()> {
    let path = Path::new(&hexfile);
    let mut fd = File::open(path)?;
    let mut buffer = Vec::new();
    fd.read_to_end(&mut buffer)?;

    match Object::parse(&buffer)? {
        Object::Elf(elf) => {
            let mut memory = HashMap::new();
            // let mut my_number: () = memory;
            memory_elf (elf, &mut memory, &buffer);
            dump_memory (&mut memory)
        }
        Object::PE(pe)           => { println!("pe is not supported."); }
        Object::Mach(mach)       => { println!("mach is not supported."); }
        Object::Archive(archive) => { println!("archive is not supported."); }
        Object::Unknown(magic)   => { println!("unknown magic: {:#x}", magic) }
    }
    Ok(())
}

fn main () {
    match env::args().nth(1) {
        None => println!("Specify filename to build."),
        Some(arg) => {
            let result = load_elf(arg);
        }
    }
}

