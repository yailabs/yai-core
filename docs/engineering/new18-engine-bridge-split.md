# NEW.18 Engine Bridge Split

Status: NEW.18 bridge consolidation wave.

NEW.18 creates the explicit C bridge boundary for the Rust operational data
engine:

```text
system/engine_bridge/
```

The Rust engine C shim implementation moves there:

```text
system/store/rust_engine_backend.c -> system/engine_bridge/rust_engine_backend.c
```

The public C header remains in `include/yai/store/rust_engine_backend.h`
because `include/yai/yai.h` exports it as part of the current C ABI surface.
NEW.18 does not change the public ABI.

## Boundary

`system/engine_bridge` owns only:

```text
Rust engine opaque handles
FFI calls into engine/yai-engine
status-code translation
allocation and caller-owned-buffer adapters
```

It does not own:

```text
store algorithms
journal mechanics
record codec semantics
graph reconstruction
index/query semantics
memory semantics
projection materialization
reconcile detection
retention or integrity policy
```

## Transitional C Data Folders

The existing C data-spine folders remain in place and keep smoke behavior
stable:

```text
system/store
system/graph
system/index
system/memory
system/projection
system/reconcile
```

Their immediate status is `keep_temporarily`. They are not final owners of the
data spine. Rust engine ownership is documented in:

```text
docs/engineering/c-data-logic-thinning-map.md
docs/engineering/engine-ownership-map.md
docs/engineering/system-ownership-map.md
```

## Audit Result

The read-only old-yai audit confirmed that substrate store, records, graph,
indexes, query, memory, views and signals mostly contain taxonomy, lifecycle,
relationship, freshness and safety contracts. Those concepts map to Rust engine
owners or to the C bridge boundary; analytics features/signals are split, with
non-core experiment material routed to future `ai-environment`.

No old-yai source is copied.

## Non-Goals

NEW.18 does not rewrite store, graph, index, memory, projection or reconcile
implementations. It does not move public headers, change daemon IPC, change
installed commands or implement the later Rust parity waves.
