# ADR 0008: C First, Rust Operational Data Engine

## Status

Accepted for DOC.NEW.1.

## Context

The future core needs a stable local ABI and daemon/carrier/control bootstrap,
but store, journal, record, graph, index/query, memory, projection, reconcile,
retention and integrity behavior will become complex. C is the right shell for
ABI and machine boundary. Rust is the right engine for the operational data
spine.

## Decision

C owns public ABI, daemon bootstrap, host carriers, control enforcement
boundary, system bridge and FFI shims. Rust may own `yai` implementation and
the operational data spine. `yai` remains a client over core primitives, not a
semantic owner. Rust owns store, journal, record codec, graph, index/query,
memory, projection, reconcile, retention and integrity behind C FFI.

Owner table:

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

## Consequences

NEW.0 and NEW.1 can remain small and ABI-stable. Rust can later own data
structures, persistence, retrieval, graph traversal, memory consolidation and
projection caches without changing public C contracts. C keeps fallback store
behavior until dual-write parity is proven.

SPINE.1 clarifies that fallback C data logic is transitional. C wrappers should
become thin as Rust data-spine parity matures.

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
