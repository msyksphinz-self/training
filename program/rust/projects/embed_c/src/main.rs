#[link(name="bfd_show", kind="static")]
#[link(name="bfd", kind="static")]
#[link(name="iberty", kind="static")]
#[link(name="z")]
extern{
    fn bfd_show(filename :String);
}

fn main() {
    unsafe { bfd_show(String::from("./target/debug/embed_c")); };
}

