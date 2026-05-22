# NEW.17 System Layout

Status: NEW.17 physical refactor wave.

NEW.17 moves the remaining C implementation from the bootstrap `lib/` root to
the target `system/` root:

```text
lib/base        -> system/base
lib/case        -> system/case
lib/control     -> system/control
lib/effect      -> system/effect
lib/graph       -> system/graph
lib/index       -> system/index
lib/ingest      -> system/ingest
lib/internal    -> system/internal
lib/memory      -> system/memory
lib/op          -> system/op
lib/projection  -> system/projection
lib/reconcile   -> system/reconcile
lib/store       -> system/store
lib/subject     -> system/subject
```

`lib/` is removed after the move. The retired `ctl/` pointer root is also
removed so the active top-level layout is `include/`, `system/`, `engine/` and
`cmd/`.

## Current Layout

```text
cmd/
  yai/
  yaid/

engine/
  yai-engine/
  yai-engine-ffi/

system/
  base/
  case/
  control/
  daemon/
  effect/
  graph/
  index/
  ingest/
  internal/
  memory/
  op/
  projection/
  reconcile/
  store/
  subject/
```

`system/` is now the C implementation root. `include/` remains the public C ABI
root. `engine/` remains the Rust operational data spine. `cmd/yai` remains the
Rust technical command and `cmd/yaid` remains the C daemon entrypoint.

## Transitional Data Logic

NEW.17 is not the ownership cleanup wave. The C data-spine folders now under:

```text
system/store
system/graph
system/index
system/memory
system/projection
system/reconcile
```

remain transitional. NEW.18 must split them into `system/engine_bridge` files,
Rust-owned `engine/yai-engine` logic, or deletion after Rust parity.

## Boundaries

NEW.17 does not change the public ABI, daemon IPC protocol, installed command
layout, Rust engine semantics or smoke behavior. It only removes the bootstrap
`lib/` container and updates build/guard/doc references to the active
`system/` location.
