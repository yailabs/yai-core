# memory

Transitional C shim.

This directory exists to keep current ABI/smoke paths working until the Rust
engine owns this plane. Do not add new long-term data-plane logic here.
Future ownership: engine/yai-engine.

Current role: C bootstrap implementation for memory candidate ABI boundaries.

Target role:

```text
system/memory -> system/engine_bridge + engine/yai-engine/src/memory
```

Owning wave: NEW.18 moves operational memory mechanics toward Rust and leaves
only bridge behavior in C.
