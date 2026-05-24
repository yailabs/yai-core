# System

Status: active implementation surface  
Owner: system  
Purpose: C host/system implementation for ABI, daemon, control, carrier and current smoke paths.  
Not source of truth for: long-term durable data-plane ownership.

`system/` contains C host/system implementation and transitional shims.

Active C implementation roots may own current behavior for base support, cases,
subjects, operations, control, effects, hot state, daemon IPC and the Rust
engine bridge.

Transitional C data shim roots keep current ABI/smoke paths working:

```text
system/store
system/graph
system/index
system/memory
system/projection
system/reconcile
```

They are not the long-term owner of record/graph/fact/memory data-plane logic.
Future durable data-plane ownership belongs under `engine/yai-engine` unless a
future roadmap wave says otherwise.
