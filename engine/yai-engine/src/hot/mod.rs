//! YAI - hot-state module
//!
//! Purpose:
//!   Group hot-state cache, snapshot and freshness helpers.
//!
//! Ownership:
//!   Rust hot-state module exports.
//!
//! Boundary:
//!   Does not make hot state durable truth.
//!
//! Status:
//!   active

pub mod freshness;
pub mod snapshot;
pub mod state;

pub use freshness::{HotFreshness, HotStaleReason};
pub use snapshot::HotSnapshot;
pub use state::HotState;
