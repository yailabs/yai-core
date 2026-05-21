# ADR 0010: Rust Data Spine Ownership

## Status

Accepted for SPINE.1.

## Context

Rust engine R1 exists behind the C ABI, while C still carries duplicated data
logic from the bootstrap implementation. The mature system needs a clear owner
for operational residue mechanics.

## Decision

Rust owns the operational data spine:

```text
store
journal
record codec
graph
index/query
memory
projection
reconcile
retention
integrity
```

C owns the system and host boundary:

```text
public ABI
daemon bootstrap
host carriers
control enforcement boundary
system bridge
FFI shims
```

C wrappers become thin. C duplicate data logic is quarantined and eventually
removed. Rust becomes the operational data spine without owning the public C
ABI or host enforcement shell.

## Consequences

Store, graph, index/query, memory, projection and reconcile mature as engine
capabilities rather than as duplicated C subsystems. Public contracts remain
stable through C ABI and protocol records.

NEW.13 maps current duplicated C data logic to:

```text
system/engine_bridge
engine/yai-engine/src/store
engine/yai-engine/src/journal
engine/yai-engine/src/record
engine/yai-engine/src/graph
engine/yai-engine/src/index
engine/yai-engine/src/query
engine/yai-engine/src/memory
engine/yai-engine/src/projection
engine/yai-engine/src/reconcile
```

NEW.18 must classify each duplicated C data file as `keep_as_bridge`,
`move_to_engine`, `replace_with_rust`, `delete_after_engine` or
`keep_temporarily`.

## Non-goals

Do not expose Rust types across public ABI. Do not move daemon bootstrap, host
carriers or control enforcement into Rust in this decision. Do not convert
SPINE.1 into an implementation wave.

## Related docs

```text
../engineering/rust-engine.md
../engineering/c-abi.md
../architecture/13-c-rust-boundary.md
../engineering/data-spine-refactor-roadmap.md
../engineering/engine-ownership-map.md
../engineering/filesystem-move-matrix.md
```
