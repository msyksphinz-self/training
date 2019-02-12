// 標準ライブラリは使用しない
#![no_std]

// mangleしない
#[no_mangle]
pub fn __start_rust() -> ! {
    // UART0の送信バッファに1文字ずつデータをストアする
    let uart0 = 0x10013000  as *mut u8;
    for c in b"Hello from Rust!".iter() {
        unsafe {
            *uart0 = *c as u8;
        }
    }

    loop {}
}

// panic発生時のハンドラ
use core::panic::PanicInfo;
#[panic_handler]
#[no_mangle]
pub fn panic(_info: &PanicInfo) -> ! {
    // 何もせず、無限ループする
    loop{}
}

