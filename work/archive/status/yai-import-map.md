# YAI Import Map

Purpose: track small, spine-safe imports from the broader `yai` repository into
`yai`.

## Rule

`yai` is a source of mature vocabulary and experiments. `yai` only imports
pieces that can become canonical local-control primitives under:

```text
include/yai/<primitive>
system/<primitive>
engine/
cmd/
tests/
```

No broad subtree copy. No hidden ownership transfer.

## Current Import

Imported concept family from `yai/src/case` and `yai/include/yai/case`:

```text
case identity
case domain
case attachment
case operational binding
projection and authority posture
```

Implemented in `yai` as:

```text
include/yai/case/case_world.h
system/case/case_world.c
record_kind: case_domain
record_kind: case_attachment
record_kind: case_binding
```

The daemon filesystem loop now emits case-world residue before subjects,
policy, decisions, receipts, graph, memory and projections.

SPINE.3R makes that ordering canonical: everything that participates in a case
enters first as case-world material, then may act as subject, source, actor,
target, projection consumer or carrier participant.

## Planned Not Created

The following remain outside this import wave:

```text
case registry persistence
process attachment registry
runtime carriers
mediation engine
enforcement engine
SHM store layer
LMDB registry/store layer
DuckDB analytical layer
Ladybug/debug layer
live projection delta layer
provider registry
agent framework
```

Next safe waves should import one vertical slice at a time, with a record kind,
C ABI primitive, daemon/smoke evidence, and CLI projection visibility.
