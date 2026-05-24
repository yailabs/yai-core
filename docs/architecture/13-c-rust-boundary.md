# C / Rust Boundary

Canonical decision:

```text
C owns public ABI, daemon bootstrap, host carriers, control enforcement boundary,
system bridge and FFI shims.
Rust owns the operational data spine: hot state mechanics, store, journal,
record codec, graph, index/query, fact derivation, memory, live projection,
reconcile, observability/evaluation, retention and integrity.
yai may be Rust, but it remains a client over primitives.
```

Rust is not the new semantic owner of the core. Public contracts stay in the C
ABI and protocol docs.

## Owner Table

| Concern | Owner |
|---|---|
| daemon lifecycle | C |
| daemon bootstrap | C |
| host carriers | C |
| control enforcement boundary | C |
| system bridge | C |
| public ABI | C |
| FFI shims | C |
| `yai` | Rust allowed, client only |
| hot state mechanics | Rust |
| store | Rust |
| journal | Rust |
| record codec | Rust |
| graph | Rust |
| index/query | Rust |
| fact derivation | Rust |
| memory | Rust |
| live projection | Rust |
| reconcile | Rust |
| observability/evaluation | Rust |
| retention | Rust |
| integrity | Rust |

## FFI Rules

```text
opaque handles
explicit alloc/free
caller-owned buffers or byte buffers
integer status codes
schema-versioned records
deterministic error strings
no Rust types across public ABI
```

## Non-Goal

Do not port the whole core to Rust before NEW.1 proves the loop. Rust is the
operational data engine, not the first daemon/control shell.

## SPINE.1 Target

```text
C wrappers become thin.
C duplicate data logic is quarantined and eventually removed.
Rust engine becomes the operational data spine.
C system remains host/control/carrier/daemon/bridge.
```

## NEW.13 Refactor Plan

NEW.13 does not move source. It defines how the boundary will be represented in
the filesystem:

```text
include/ = public C ABI contracts
system/  = C daemon, host, carriers, control boundary and FFI bridge
engine/  = Rust operational data spine
cmd/     = yai and yaid entrypoints
```

The current C data logic in `system/store`, `system/graph`, `system/index`,
`system/memory`, `system/projection` and `system/reconcile` is transitional.
NEW.18 creates `system/engine_bridge`, moves the Rust engine C shim there and
classifies the remaining C data logic as `keep_temporarily` versus Rust
`engine/yai-engine` target ownership.

## NEW.14 Engine Move

NEW.14 moved only the Rust engine crates:

```text
crates/yai-engine     -> engine/yai-engine
crates/yai-engine-sys -> engine/yai-engine-ffi
```

NEW.15 then moved the command to `cmd/yai` and removed `crates/`. The C ABI, C
sources, daemon entrypoint and smoke semantics are unchanged across NEW.14 and
NEW.15.

NEW.16 moved `yaid` into `cmd/yaid` and daemon support into `system/daemon`.
NEW.17 moved the remaining C implementation into `system/` without changing the
public C ABI or daemon protocol.
NEW.18 moved `system/store/rust_engine_backend.c` to
`system/engine_bridge/rust_engine_backend.c` while keeping the public shim
header under `include/yai/store/`.

## SPINE.3R Data-Plane Boundary

```text
shared memory = hot state, not truth
journal = replay
LMDB = durable record plane
Ladybug = operational graph
DuckDB = derived facts / analytics
Rust engine = data-plane materialization and query
C system = host boundary, daemon, carrier, control shell, FFI bridge
```

SPINE.3R documents this target only. It does not implement shared memory,
LMDB, Ladybug or DuckDB.

## SPINE.4 Observability Boundary

Operational observability/evaluation is a data-plane concern. Rust is the
target owner for trace materialization, timing samples, analytics facts,
freshness state, rebuild reports, model behavior facts and memory quality
facts.

C system code may expose status, daemon diagnostics, carrier timing and public
ABI handles. It must not become the long-term analytics or case-view quality
engine.
