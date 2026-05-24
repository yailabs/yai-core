# projection

Transitional C shim.

This directory exists to keep current ABI/smoke paths working until the Rust
engine owns this plane. Do not add new long-term data-plane logic here.
Future ownership: engine/yai-engine.

Current role: C bootstrap implementation for controlled projections. Projection
mechanics move toward Rust-owned data spine code while C keeps only ABI/bridge
behavior. No UI state belongs here.

Target role:

```text
system/projection -> system/engine_bridge + engine/yai-engine/src/projection
```

Owning wave: NEW.18 moves projection materialization toward Rust and leaves
only bridge behavior in C.
