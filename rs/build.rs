// build.rs

use std::env;

extern crate binder;
extern crate cheddar;

fn main() {
     let dir = match env::var("CMAKE_BINARY_DIR") {
	Ok(v) => v,
	Err(_) => "../build".to_string()
    };

    let c99 = cheddar::Header::c99()
         .name("rust-lib.h");

    binder::Binder::new().unwrap()
        .output_directory(dir)
        .register(c99) 
        .run_build();
}

