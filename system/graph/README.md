# graph

Transitional role: C bootstrap implementation for graph edge contracts and
reconstruction hooks.

Target role:

```text
system/graph -> system/engine_bridge + engine/yai-engine/src/graph
```

Owning wave: NEW.18 replaces duplicated graph data logic with Rust engine
ownership while preserving C ABI bridge behavior.
