# Rust Operational Data Engine

Rust is the advanced operational data engine behind the C ABI. Rust is not the
new core owner.

## 1. Canonical Decision

```text
C owns core ABI and daemon/carrier/control bootstrap.
Rust may own yaictl implementation and operational data engine.
yaictl remains a client over core primitives, not a semantic owner.
Rust owns advanced store/index/graph/memory/projection/query behind C FFI.
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
journal indexing
graph edge storage and traversal
memory recall and consolidation
projection materialization caches
query planning
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
| `yaictl` | Rust allowed, client only |
| ctl semantic authority | none; `yaictl` is client over primitives |
| journal backend advanced | Rust |
| graph traversal | Rust |
| memory consolidation | Rust |
| projection cache | Rust |
| query planning | Rust |
| fallback store backend | C |

## 4. First Crate Shape

```text
crates/
├── yai-core-engine/
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
│       ├── retention.rs
│       └── integrity.rs
└── yai-core-engine-sys/
```

The crate can exist in R0 without being production path. It should first prove
that it can round-trip schema-versioned records behind the C ABI.

## 5. First Backend

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

## 6. C Fallback

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

## 7. Dual-Write Sequence

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

## 8. Adoption Sequence

```text
R0: crate exists, not production path
R1: append-only store backend
R2: dual-write C jsonl + Rust store
R3: compare tail/count/integrity
R4: graph edge storage
R5: memory recall/consolidation
R6: index/query planning
R7: projection materialization caches
R8: C store backend fallback only
```

## 9. FFI Rules

```text
opaque handles
explicit alloc/free
caller-owned buffers or byte buffers
integer status codes
schema-versioned records
deterministic error strings
no Rust types across public ABI
```

## 10. Test Requirements

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
