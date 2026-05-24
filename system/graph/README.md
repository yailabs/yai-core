# graph

Transitional C shim.

This directory exists to keep current ABI/smoke paths working until the Rust
engine owns this plane. Do not add new long-term data-plane logic here.
Future ownership: engine/yai-engine.

Current role: C bootstrap implementation for graph edge contracts and
reconstruction hooks.

Target role:

```text
system/graph -> system/engine_bridge + engine/yai-engine/src/graph
```

Owning wave: NEW.18 replaces duplicated graph data logic with Rust engine
ownership while preserving C ABI bridge behavior.
