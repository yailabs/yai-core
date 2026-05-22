# projection

Transitional role: C bootstrap implementation for controlled projections.

Target role: projection mechanics move toward Rust-owned data spine code while
C keeps only ABI/bridge behavior. No UI state belongs here.

Target role:

```text
system/projection -> system/engine_bridge + engine/yai-engine/src/projection
```

Owning wave: NEW.18 moves projection materialization toward Rust and leaves
only bridge behavior in C.
