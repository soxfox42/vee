use std::{fs::File, io::Read, process};

use vee::Vee;

fn main() {
    let args: Vec<String> = std::env::args().collect();
    if args.len() < 2 {
        eprintln!("Missing ROM name.");
        process::exit(1);
    }
    let filename = &args[1];

    let rom = {
        let Ok(mut file) = File::open(filename) else {
            eprintln!("ROM {filename} not found.");
            process::exit(1);
        };
        let mut buf = Vec::new();
        file.read_to_end(&mut buf).unwrap();
        buf
    };

    let mut vee = Vee::new(&rom);
    vee.tick();
}
