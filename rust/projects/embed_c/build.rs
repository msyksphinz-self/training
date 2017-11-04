extern crate gcc;

fn main(){
    gcc::Build::new()
        .file("src/c/bfd_show.c")
        .include("src")
        .compile("libbfd_show.a");
    println!("cargo:rustc-link-search=native=./lib")
}
