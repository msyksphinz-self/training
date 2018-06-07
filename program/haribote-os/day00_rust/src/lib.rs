#![feature(lang_items)]
#![feature(start)]
#![feature(panic_implementation)]
#![no_std]
#![feature(asm)]
extern crate rlibc;

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
	for addr in 0xa0000..(0xb0000-1) {
		let vram: *mut u8 = addr as *mut u8;
		unsafe { *vram = 0xf; }
	}
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
