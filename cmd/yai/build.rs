use std::env;
use std::path::PathBuf;
use std::process::Command;

fn main() {
    let manifest_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").expect("CARGO_MANIFEST_DIR"));
    let repo_root = manifest_dir.join("../..");
    let linenoise = repo_root.join("vendor/linenoise/linenoise.c");
    let out_dir = PathBuf::from(env::var("OUT_DIR").expect("OUT_DIR"));
    let object = out_dir.join("linenoise.o");
    let library = out_dir.join("liblinenoise.a");
    let cc = env::var("CC").unwrap_or_else(|_| "cc".to_string());
    let ar = env::var("AR").unwrap_or_else(|_| "ar".to_string());

    let compile_status = Command::new(&cc)
        .arg("-std=gnu99")
        .arg("-D_DEFAULT_SOURCE")
        .arg("-O2")
        .arg("-c")
        .arg(&linenoise)
        .arg("-o")
        .arg(&object)
        .status()
        .expect("failed to invoke C compiler for linenoise");
    if !compile_status.success() {
        panic!("failed to compile vendored linenoise");
    }

    let archive_status = Command::new(&ar)
        .arg("rcs")
        .arg(&library)
        .arg(&object)
        .status()
        .expect("failed to invoke archiver for linenoise");
    if !archive_status.success() {
        panic!("failed to archive vendored linenoise");
    }

    println!("cargo:rustc-link-search=native={}", out_dir.display());
    println!("cargo:rustc-link-lib=static=linenoise");
    println!("cargo:rerun-if-changed={}", linenoise.display());
    println!(
        "cargo:rerun-if-changed={}",
        repo_root.join("vendor/linenoise/linenoise.h").display()
    );
}
