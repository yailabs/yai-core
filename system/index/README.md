# index

Transitional C shim.

This directory exists to keep current ABI/smoke paths working until the Rust
engine owns this plane. Do not add new long-term data-plane logic here.
Future ownership: engine/yai-engine.

Current role: C bootstrap implementation for initial query and retrieval
boundaries.

Target role:

```text
system/index -> system/engine_bridge + engine/yai-engine/src/index/query
```

Owning wave: NEW.18 moves query/index ownership toward Rust and leaves only
bridge behavior in C.
