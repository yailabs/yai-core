# Engine

Status: active implementation surface
Owner: engine
Purpose: Rust operational data engine and future durable data-plane owner.
Not source of truth for: C ABI compatibility or daemon process lifecycle.

`engine/` is the Rust operational data engine and future owner of durable
data-plane logic.

This root owns the Rust data spine workspace after the NEW.14 move:

```text
engine/yai-engine
engine/yai-engine-ffi
```

NEW.15 moved the command to `cmd/yai` and removed `crates/`.
