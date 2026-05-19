//! Future FFI boundary notes.
//!
//! NEW.1 does not expose Rust types through the public C ABI.
//! Later FFI must use:
//!
//! - opaque handles only
//! - explicit alloc/free
//! - schema-versioned records
//! - byte buffers or caller-owned buffers
//! - integer status codes

pub const FFI_BOUNDARY_RULE: &str = "no Rust types across public C ABI";
