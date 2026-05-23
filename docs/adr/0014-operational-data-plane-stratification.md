# ADR 0014: Operational Data Plane Stratification

## Status

Accepted for SPINE.3R.

## Context

The data spine already separates store, graph, index/query, memory, projection
and reconcile. The next build/guard realignment needs a clearer physical data
plane map before backend rules are tightened.

## Decision

YAI Core uses stratified operational data planes:

```text
hot/shared memory
journal replay
LMDB record plane
Ladybug graph plane
DuckDB fact plane
memory plane
projection plane
reconcile plane
```

Shared memory is hot state, not truth. Journal is replay. LMDB is durable
record lookup. Ladybug is operational graph. DuckDB is derived analytical fact
material. Rust engine owns data-plane materialization and query. C system owns
host boundary, daemon, carrier, control shell and FFI bridge.

## Consequences

NEW.19 must not write guards as if the target is only filesystem layout. It
must align to `system = host/control/carrier/daemon/bridge` and `engine =
hot/store/journal/record/graph/index/query/memory/projection/reconcile`.

SPINE.3R does not implement LMDB, DuckDB, Ladybug or shared memory.
