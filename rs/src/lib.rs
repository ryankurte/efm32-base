#![feature(lang_items)]
#![feature(compiler_builtins_lib)]
#![no_std]

#[repr(C)]
pub struct ExampleStruct {
  a: u32,
  b: u32
}

#[no_mangle]
pub extern fn example_one(a: u32, b: u32) -> u32 {
    a + b
}

impl ExampleStruct {
    #[no_mangle]
    pub extern fn example_two(&mut self, _data: &[u8]) {

    }
}

#[lang = "panic_fmt"]
#[no_mangle]
unsafe extern "C" fn panic_fmt(
    _args: core::fmt::Arguments,
    _file: &'static str,
    _line: u32,
    _col: u32,
) -> ! {
    loop {}
}

#[lang = "eh_personality"]
#[no_mangle]
extern fn eh_personality() {}

#[cfg(test)]
mod tests {
    use example_one;

    #[test]
    fn it_works() {
        assert_eq!(example_one(1u32, 3u32), 4);
    }
}

