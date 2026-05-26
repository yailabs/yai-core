# Graph Persistence / RuntimeGraph

Status: SPINE.40 Graph Persistence / RuntimeGraph Doctrine + Schema.

## Executive Summary

SPINE.40 defines graph persistence and RuntimeGraph as separate surfaces.
SPINE.41 adds an active-minimal `yai.graph_relation.v1` relation write path
from LMDB records.
SPINE.42 adds an active-minimal RuntimeGraph in-memory working set loaded from
graph relations.

Graph persistence is the future durable relation truth / rebuild contract.
Graph persistence owns durable typed relations. RuntimeGraph is the future
in-memory active case working set. HNSW later attaches to retrieval
units/runtime nodes as candidate retrieval, not graph truth. HNSW is not graph truth.
Context Compiler later renders controlled projection/model context from
RuntimeGraph, retrieval candidates, facts, policies and token budgets.

## Command Surface

Added commands:

```bash
yai graph schema
yai graph runtime-status
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref> --limit 20
yai graph runtime-load --case <case_ref>
yai graph runtime-summary --case <case_ref>
```

Changed commands:

```text
none
```

Removed/deprecated commands:

```text
none
```

## Doctrine

```text
Persistent truth on disk.
Computational shape in memory.
Vector proximity as candidate discovery.
Compiled projection as model-facing view.
```

```text
LMDB stores records.
Graph persistence stores typed relations.
RuntimeGraph computes over the active case.
HNSW finds candidate nodes.
Context Compiler renders controlled views.
Projection does not disappear.
```

## RuntimeGraph Boundary

RuntimeGraph is the in-memory active case working set. It may later hold
runtime nodes, runtime edges, generation counters, dirty deltas, adjacency
indexes, reverse adjacency indexes, case indexes, subject indexes, policy
indexes and retrieval-unit bindings.

SPINE.40 does not implement that working set. It exposes schema/status only.
SPINE.41 still does not implement RuntimeGraph. It writes durable graph
relations to `lmdb_graph_relations_v0`.
SPINE.42 implements RuntimeGraph as a per-command ephemeral working set. It
writes no durable truth and does not start a resident service.

## CLI Output

`yai graph schema` must expose:

```text
graph_schema:
node_kinds:
edge_kinds:
relation_write_path: active_minimal
```

`yai graph runtime-status` must expose:

```text
runtime_graph:
status: active_minimal
role: in_memory_active_case_working_set
working_set: per_command_ephemeral
resident_service: planned
source: graph_relations
durable_truth: graph_persistence
hnsw: future_candidate_index
context_compiler: future_consumer
relation_write_path: active_minimal
graph_store: lmdb_graph_relations_v0
```

`yai graph materialize` must expose:

```text
schema: yai.graph_relation.v1
relations_written:
relations_duplicate:
relations_skipped:
runtime_graph_updated: false
```

`yai graph relations` must expose `relation_id`, `edge_kind` and
`source_record_id`. Required edge examples include `decision_controls_attempt`
and `receipt_records_effect`.

## Validation

```bash
make check-graph-runtimegraph-doctrine
make smoke-spine40
target/debug/yai graph schema
target/debug/yai graph runtime-status
```

Smoke verifies graph node kind roundtrip, graph edge kind roundtrip, CLI schema
output, runtime boundary output and no RuntimeGraph implementation claim.

## Non-Goals

```text
No full graph persistence engine.
No Ladybug integration yet.
No resident RuntimeGraph service.
No graph rebuild.
No HNSW.
No context compiler.
No memory consolidation.
No DuckDB facts.
No carrier/provider behavior changes.
No docs/manuals path updates.
```
