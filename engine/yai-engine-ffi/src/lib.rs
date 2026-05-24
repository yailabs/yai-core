//! Sys crate marker for the YAI engine FFI.
//!
//! NEW.10 exposes Rust engine functions through opaque handles, explicit
//! alloc/free, integer status codes and schema-versioned byte records.

pub const ABI_RULE: &str = "opaque handles only; no Rust types across public C ABI";
