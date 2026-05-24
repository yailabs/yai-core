# reconcile

Transitional C shim.

This directory exists to keep current ABI/smoke paths working until the Rust
engine owns this plane. Do not add new long-term data-plane logic here.
Future ownership: engine/yai-engine.

Current role: C bootstrap implementation for divergence and recovery posture
records.

Target role:

```text
system/reconcile -> system/engine_bridge + engine/yai-engine/src/reconcile
```

Owning wave: NEW.18 moves reconcile detection toward Rust. Recovery execution
does not enter the data spine.
