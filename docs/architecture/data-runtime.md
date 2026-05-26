# Data Runtime

YAI separates durable truth from runtime computation.

```text
durable truth: journal, LMDB records, graph relations
runtime computation: hot state, RuntimeGraph, projections
planned analytics: DuckDB facts, HNSW/retrieval, memory, Context Compiler
```

## Planes

- Hot state is a live cache and snapshot, not truth.
- Journal is append-only replay and audit residue.
- LMDB is durable indexed record lookup.
- Graph persistence stores durable typed relations as `yai.graph_relation.v1`.
- RuntimeGraph loads active-case graph relations into a per-command in-memory
  working set.
- RuntimeGraph rebuild can replay journal material into LMDB, materialize graph
  relations and load a runtime graph report.

RuntimeGraph is not durable truth. It is bounded active-case computation over
relations that can be rebuilt from durable planes.

## Planned Planes

DuckDB facts are planned for analytical facts, not record truth. HNSW and
retrieval are planned candidate-selection surfaces, not graph truth. Context
Compiler is planned as a controlled rendering surface over case material, not a
replacement for projection, policy or memory.

