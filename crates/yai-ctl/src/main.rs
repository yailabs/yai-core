const VERSION: &str = env!("CARGO_PKG_VERSION");

fn print_info() {
    println!("yaictl: technical YAI Core control client");
    println!("status: NEW.1");
    println!("ownership: Rust client over C-defined core primitives");
    println!("daemon_ipc: not implemented");
}

fn print_doctor() {
    println!("yaictl doctor: ok");
    println!("public_semantics: C ABI + core docs");
    println!("rust_role: ctl client and operational data engine skeleton");
}

fn print_usage() {
    println!("usage: yaictl [--version|info|doctor]");
}

fn main() {
    let mut args = std::env::args().skip(1);
    match args.next().as_deref() {
        Some("--version") | Some("version") => println!("yaictl {}", VERSION),
        Some("info") => print_info(),
        Some("doctor") => print_doctor(),
        Some(_) => {
            print_usage();
            std::process::exit(2);
        }
        None => print_usage(),
    }
}
