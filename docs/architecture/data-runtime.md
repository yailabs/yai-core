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

DuckDB facts are planned for analytical facts, not record truth. SPINE.46 now
starts as DuckDB Fact Plane Doctrine + Bitemporal Schema: a fact is not just a
row, it is a temporally scoped, provenance-bearing assertion with
`transaction_time`, `valid_time_start`, `valid_time_end`, `known_at`,
`revision_of`, `superseded_by` and `retracted_by` posture.

HNSW and retrieval are planned candidate-selection surfaces, not graph truth.
Native retrieval, attached retrieval and federated retrieval produce retrieval
candidates. Retrieval candidates are not evidence until resolved. External
retrieval cannot enter model context without receipt/scope/provenance gating,
and external retrieval cannot become case material without explicit import.

Context Compiler is planned as a controlled rendering surface over case
material, not a replacement for projection, policy or memory. ContextFrame is
not truth. ContextDelta is not memory. CaseModelSession is not authority. KV
cache is not YAI memory.

Graph corrections are planned as append-only patches, not destructive edits.
Temporal Graph Revision, Graph Patch Operation, Dependency Closure and
Counterfactual case branches must remain stale-aware so projections, context,
memory, retrieval and reconcile can invalidate dependent material correctly.
