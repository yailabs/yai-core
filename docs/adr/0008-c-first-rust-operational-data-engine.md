# ADR 0008: C First, Rust Operational Data Engine

## Status

Accepted for DOC.NEW.1.

## Context

The future core needs a stable local ABI and daemon/carrier/control bootstrap,
but advanced store, graph, memory, projection and query behavior will become
complex. C is the right first shell for ABI and machine boundary. Rust is the
right later engine for operational data structures.

## Decision

C owns core ABI and daemon/carrier/control bootstrap. Rust may own `yaictl`
implementation and the operational data engine. `yaictl` remains a client over
core primitives, not a semantic owner. Rust owns advanced
store/index/graph/memory/projection/query behind C FFI.

Owner table:

| Concern | Owner |
|---|---|
| daemon lifecycle | C |
| carrier v0 | C |
| control v0 | C |
| public ABI | C |
| `yaictl` | Rust allowed, client only |
| advanced store engine | Rust |
| graph/index/memory | Rust |
| projection cache | Rust |

## Consequences

NEW.0 and NEW.1 can remain small and ABI-stable. Rust can later own data
structures, persistence, retrieval, graph traversal, memory consolidation and
projection caches without changing public C contracts. C keeps fallback store
behavior until dual-write parity is proven.

## Non-goals

Do not port daemon, carrier bootstrap or control v0 to Rust first. Do not let
Rust own case/control semantics. Do not expose Rust types across public ABI.

## Supersedes

Both extremes: all-C forever and Rust-as-new-core-owner.

## Related docs

```text
../engineering/c-abi.md
../engineering/rust-engine.md
../architecture/13-c-rust-boundary.md
```
