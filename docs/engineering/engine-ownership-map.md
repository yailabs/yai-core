# Engine Ownership Map

Status: NEW.13 planning artifact.

`engine/` is the Rust operational data spine. It owns residue mechanics behind
the public C ABI and system bridge. It does not own daemon bootstrap, host
carriers, policy authority or public ABI semantics.

## Target Shape

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

## Owned Primitives

| Engine area | Owns | Does not own |
|---|---|---|
| `residue/` | common residue refs, record identity mechanics and case-bound residue posture | public C ABI authority |
| `store/` | durable residue persistence | memory semantics |
| `journal/` | append/tail/replay mechanics | daemon lifecycle |
| `record/` | record model and codec mechanics | policy authority |
| `graph/` | causality and reconstruction over residue | legacy lineage root |
| `index/` | operational indexes and freshness metadata | RAG product semantics |
| `query/` | operational access over case-bound residue | projection authority |
| `memory/` | receipt-backed, graph-derived, policy-aware operational experience | chat history |
| `projection/` | controlled read-model materialization | UI state |
| `reconcile/` | expected-vs-observed mismatch detection and recovery posture | recovery execution |
| `retention/` | retention and forgetting mechanics | external legal authority |
| `integrity/` | hashes, parity checks and residue consistency | trust over unbound external claims |
| `ffi/` | Rust-side FFI adapter mechanics | public C allocation rules |

## Current Bootstrap Sources

| Current source | Target engine area | Notes |
|---|---|---|
| `crates/yai-core-engine/src/store.rs` | `engine/yai-engine/src/store/` | NEW.14 path move. |
| `crates/yai-core-engine/src/journal.rs` | `engine/yai-engine/src/journal/` | NEW.14 path move. |
| `crates/yai-core-engine/src/record.rs` | `engine/yai-engine/src/record/` | NEW.14 path move. |
| `crates/yai-core-engine/src/graph.rs` | `engine/yai-engine/src/graph/` | NEW.14 path move. |
| `crates/yai-core-engine/src/index.rs` | `engine/yai-engine/src/index/` | NEW.14 path move. |
| `crates/yai-core-engine/src/query.rs` | `engine/yai-engine/src/query/` | NEW.14 path move. |
| `crates/yai-core-engine/src/memory.rs` | `engine/yai-engine/src/memory/` | NEW.14 path move. |
| `crates/yai-core-engine/src/projection.rs` | `engine/yai-engine/src/projection/` | NEW.14 path move. |
| `crates/yai-core-engine/src/reconcile.rs` | `engine/yai-engine/src/reconcile/` | NEW.14 path move. |
| `crates/yai-core-engine/src/ffi.rs` | `engine/yai-engine/src/ffi/` | NEW.14 path move. |
| `crates/yai-core-engine-sys/src/lib.rs` | `engine/yai-engine-ffi/src/lib.rs` | Rust FFI crate, not public C ABI owner. |

## Old-YAI Evidence Routes

| Old-yai material | Engine route | Classification |
|---|---|---|
| `../yai/src/substrate/store/*` | `store/`, `journal/`, `integrity/` | mine concepts; no backend port |
| `../yai/src/substrate/records/*` | `record/`, `residue/` | mine taxonomy and lifecycle evidence |
| `../yai/src/substrate/graph/*` | `graph/` | mine causality/reconstruction contracts |
| `../yai/src/substrate/indexes/*` | `index/` | mine freshness and retrieval taxonomy |
| `../yai/src/substrate/query/*` | `query/` | mine read posture and safety contracts |
| `../yai/src/substrate/memory/*` | `memory/` | mine lifecycle/privacy/consolidation evidence |
| `../yai/src/substrate/views/*` | `projection/` | mine controlled read-model materialization |
| `../yai/src/substrate/signals/*` | `reconcile/`, `memory/` | mine contradiction, missingness and confidence posture |
| `../yai/src/lineage/*` | `graph/`, `memory/`, `projection/` | mine graph-supported memory and audit projection |
| `../yai/src/analytics/*` | `index/`, `memory/`, `reconcile/` or `ai-environment` | mine features/signals; externalize experiments |

## Bridge Contract

The engine is reached from C through `system/engine_bridge` and public headers
under `include/yai/`. The bridge may own handles, allocation adapters and
status-code translation. It must not reimplement durable data logic once Rust
has parity.

## Acceptance Target

By NEW.21 the repository layout can still have C bridge code, but duplicated C
data logic must be marked for Rust replacement or deletion in NEW.22 through
NEW.30.
