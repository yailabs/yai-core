# ADR 0005: Store / Index / Graph / Memory / Projection Split

## Status

Accepted for DOC.NEW.1.

## Context

The old `substrate` root mixed persistence, records, sources, indexes, graph,
memory, query, views, retention and signals. Those concerns interact, but they
do not have the same ownership or truth model.

## Decision

The old `substrate` root is split into separate future roots:

```text
store
index
graph
memory
projection
```

## Consequences

Persistence, retrieval, reconstruction, continuity and read models have separate
ownership. They can later share a Rust operational data engine without
collapsing back into one semantic root.

## Non-goals

Do not keep `substrate` as a final root. Do not use `views` as the future core
root name; use `projection`. Do not make memory raw storage or projection source
truth.

## Supersedes

The old `src/substrate` root as a future core root.

## Related docs

```text
../architecture/08-store-index-graph-model.md
../architecture/09-memory-model.md
../architecture/10-projection-model.md
../engineering/rust-engine.md
```
