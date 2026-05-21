# C / Rust Boundary

Canonical decision:

```text
C owns public ABI, daemon bootstrap, host carriers, control enforcement boundary,
system bridge and FFI shims.
Rust owns the operational data spine: store, journal, record codec, graph,
index/query, memory, projection, reconcile, retention and integrity.
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
| store | Rust |
| journal | Rust |
| record codec | Rust |
| graph | Rust |
| index/query | Rust |
| memory | Rust |
| projection | Rust |
| reconcile | Rust |
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

The current C data logic in `lib/store`, `lib/graph`, `lib/index`,
`lib/memory`, `lib/projection` and `lib/reconcile` is transitional. NEW.18 must
split it into `system/engine_bridge` versus Rust `engine/yai-engine` ownership.
