#![feature(lang_items)]
#![feature(start)]
#![feature(panic_implementation)]
#![no_std]
#![feature(asm)]
extern crate rlibc;

use core::panic::PanicInfo;

#[no_mangle]
extern {
    fn io_out8 (port:i32, data:u8);
    fn io_load_eflags () -> u32;
    fn io_cli ();
    fn io_store_eflags (eflags:u32);
}

#[no_mangle]
fn hlt() {
    unsafe {
        asm!("hlt");
    }
}

#[no_mangle]
#[start]
pub extern fn init_os() {
    init_palette();
    // for addr in 0xa0000..(0xb0000-1) {
    //     let vram: *mut u8 = addr as *mut u8;
    //     unsafe { *vram = 0xf; }
    // }
    loop {
        hlt()
    }
}


fn set_palette (start: i32, end: i32, rgb: &[u8;16*3])
{
    unsafe {
        let eflags = io_load_eflags();
        io_cli ();
        io_out8 (0x03c8, start as u8);
        for i in start..(end-1) {
            io_out8 (0x03c9, (rgb[(i*2+0) as usize] / 4) as u8);
            io_out8 (0x03c9, (rgb[(i*2+1) as usize] / 4) as u8);
            io_out8 (0x03c9, (rgb[(i*2+2) as usize] / 4) as u8);
            // io_out8 (0x03c9, 0x00);
            // io_out8 (0x03c9, 0x00);
            // io_out8 (0x03c9, 0x00);
        }
        io_store_eflags (eflags);
    }
    return;
}


fn init_palette() {
    let table_rgb: [u8;16*3] = [
        0x00, 0x00, 0x00,
        0xff, 0x00, 0x00,
        0x00, 0xff, 0x00,
        0xff, 0xff, 0x00, /* 3:明るい黄色 */
        0x00, 0x00, 0xff, /* 4:明るい青 */
        0xff, 0x00, 0xff, /* 5:明るい紫 */
        0x00, 0xff, 0xff, /* 6:明るい水色 */
        0xff, 0xff, 0xff, /* 7:白 */
        0xc6, 0xc6, 0xc6, /* 8:明るい灰色 */
        0x84, 0x00, 0x00, /* 9:暗い赤 */
        0x00, 0x84, 0x00, /* 10:暗い緑 */
        0x84, 0x84, 0x00, /* 11:暗い黄色 */
        0x00, 0x00, 0x84, /* 12:暗い青 */
        0x84, 0x00, 0x84, /* 13:暗い紫 */
        0x00, 0x84, 0x84, /* 14:暗い水色 */
        0x84, 0x84, 0x84  /* 15:暗い灰色 */
    ];
    set_palette (0, 15, &table_rgb);

    return;
}

#[lang = "eh_personality"]
extern fn eh_personality() {}

#[panic_implementation]
#[no_mangle]
pub fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
