//! Placeholder sys crate for the future YAI Core engine FFI.
//!
//! NEW.1 intentionally exposes no Rust types across the public C ABI.
//! Future bindings must use opaque handles, explicit alloc/free, integer status
//! codes and schema-versioned byte records.

pub const ABI_RULE: &str = "opaque handles only; no Rust types across public C ABI";
