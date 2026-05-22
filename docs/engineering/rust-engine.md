# Rust Operational Data Engine

Rust is the operational data spine behind the C ABI. Rust is not the new core
owner and does not own host enforcement.

## 1. Canonical Decision

```text
C owns the system / host boundary / ABI / daemon / carrier / control
enforcement shell.
Rust may own yai implementation and operational data spine.
yai remains a client over core primitives, not a semantic owner.
Rust owns store, journal, record codec, graph, index/query, memory, projection,
reconcile, retention and integrity behind C FFI.
```

C ABI owns public semantics. Rust can own data structures and algorithms behind
that ABI.

## 2. Semantic Boundary

Rust engine does not own:

```text
case/control semantics
public ABI truth
policy authority
carrier bootstrap
daemon lifecycle
integration guarantee labels
```

Rust engine owns or can later own:

```text
append-only record storage
journal mechanics
record codec
journal indexing
graph edge storage and traversal
memory recall and consolidation
projection materialization caches
query planning
reconcile detection
integrity checks
retention mechanics
```

## 3. Ownership Table

| Concern | Owner |
|---|---|
| daemon lifecycle | C |
| daemon bootstrap ABI | C |
| carrier v0 | C |
| control v0 | C |
| public headers | C ABI |
| serialized record schemas | C ABI + protocol docs |
| `yai` | Rust allowed, client only |
| ctl semantic authority | none; `yai` is client over primitives |
| store | Rust |
| journal | Rust |
| record codec | Rust |
| journal backend advanced | Rust |
| graph traversal | Rust |
| index/query | Rust |
| memory consolidation | Rust |
| projection cache | Rust |
| reconcile detection | Rust |
| retention | Rust |
| integrity | Rust |
| query planning | Rust |
| fallback store backend | C, transitional |

## 4. Target Engine Shape

SPINE.1 target shape:

```text
engine/
├── Cargo.toml
├── yai-engine/
│   ├── Cargo.toml
│   └── src/
│       ├── lib.rs
│       ├── residue/
│       ├── store/
│       ├── journal/
│       ├── record/
│       ├── graph/
│       ├── index/
│       ├── query/
│       ├── memory/
│       ├── projection/
│       ├── reconcile/
│       ├── retention/
│       ├── integrity/
│       └── ffi/
├── yai-engine-ffi/
│   ├── Cargo.toml
│   └── src/lib.rs
└── README.md
```

NEW.14 moved the Rust operational data engine out of `crates/` and into
`engine/`. NEW.15 moved the command to `cmd/yai` and removed `crates/`.

## 5. Current Engine Crate Shape

```text
engine/
├── Cargo.toml
├── yai-engine/
│   └── src/
│       ├── ffi.rs
│       ├── store.rs
│       ├── journal.rs
│       ├── record.rs
│       ├── graph.rs
│       ├── index.rs
│       ├── memory.rs
│       ├── projection.rs
│       ├── query.rs
│       ├── query.rs
│       └── reconcile.rs
└── yai-engine-ffi/
```

The crate can exist in R0 without being production path. It should first prove
that it can round-trip schema-versioned records behind the C ABI.

## 6. First Backend

The first Rust backend should be append-only and boring:

```text
record append
record read by id
journal tail
count by record kind
integrity hash check
```

No graph optimization, semantic memory or projection cache should block the
first backend.

## 7. C Fallback

C keeps a simple fallback store backend until Rust has proven:

```text
append parity
tail parity
count parity
integrity parity
fixture parity
crash recovery posture
```

Fallback does not mean C owns the advanced data future. It means NEW.1 remains
small, debuggable and ABI-stable while Rust matures.

SPINE.1 clarifies that C duplicate data logic is transitional. It should be
quarantined and thinned once the Rust engine proves parity.

## 8. Dual-Write Sequence

```text
C jsonl journal append
Rust backend append
compare tail refs
compare counts by record kind
compare integrity hashes
report divergence
fail back to C backend when configured
```

Dual-write is temporary. Its purpose is proving parity, not becoming a permanent
architecture.

## 9. Adoption Sequence

```text
R0: crate exists, not production path
R1: append-only residue mirror
R2: Rust store backend
R3: Rust record codec / journal
R4: Rust query/index
R5: Rust graph reconstruction
R6: Rust memory consolidation
R7: Rust projection materialization
R8: Rust reconcile detection
R9: C wrapper thinning
```

## 10. FFI Rules

```text
opaque handles
explicit alloc/free
caller-owned buffers or byte buffers
integer status codes
schema-versioned records
deterministic error strings
no Rust types across public ABI
```

## 11. Test Requirements

Each Rust adoption step must add fixtures that compare:

```text
record bytes
record count
journal tail
hash/integrity status
graph edge round trip
memory recall scope
projection freshness
fallback behavior
```

Rust is accepted only when it can make operational residue more reliable without
moving core semantics out of the public ABI.

## 12. NEW.10 R1 Integration

NEW.10 starts the R1 path without replacing the C journal. The Rust engine can
consume `yai.store.record.v0` JSONL residue and expose counts, kind queries and
projection summary JSON behind FFI.

R1 exports only:

```text
opaque handle
JSON record append
record count
kind count
kind query count
projection summary JSON
caller-owned output buffer
```

The C shim lives under the store boundary:

```text
include/yai/store/rust_engine_backend.h
system/store/rust_engine_backend.c
```

This is a mirror/backend path, not semantic promotion. NEW.17 moved it under
`system/store`; NEW.18 decides whether it stays as bridge code or moves to
`system/engine_bridge`. C remains the public ABI owner and the C file-backed
journal remains tested as the default path.
