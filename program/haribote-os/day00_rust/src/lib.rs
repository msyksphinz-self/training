#![feature(lang_items)]
#![feature(start)]
#![feature(panic_implementation)]
#![no_std]
#![feature(asm)]

use core::panic::PanicInfo;

#[no_mangle]
fn hlt() {
    unsafe {
        asm!("hlt");
    }
}

#[no_mangle]
#[start]
pub extern fn init_os() {
    loop {
        hlt()
    }
}

#[lang = "eh_personality"]
extern fn eh_personality() {}

#[panic_implementation]
#[no_mangle]
pub fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
