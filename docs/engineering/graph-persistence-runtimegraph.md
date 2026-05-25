# Graph Persistence / RuntimeGraph

Status: SPINE.40 Graph Persistence / RuntimeGraph Doctrine + Schema.

## Executive Summary

SPINE.40 defines graph persistence and RuntimeGraph as separate surfaces.

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

## CLI Output

`yai graph schema` must expose:

```text
graph_schema:
node_kinds:
edge_kinds:
graph_store_claim: none
```

`yai graph runtime-status` must expose:

```text
runtime_graph:
status: planned
role: in_memory_active_case_working_set
durable_truth: graph_persistence
hnsw: future_candidate_index
context_compiler: future_consumer
graph_store_claim: none
```

## Validation

```bash
make check-graph-runtimegraph-doctrine
make smoke-spine40
target/debug/yai graph schema
target/debug/yai graph runtime-status
```

Smoke verifies graph node kind roundtrip, graph edge kind roundtrip, CLI schema
output, runtime boundary output and no graph persistence implementation claim.

## Non-Goals

```text
No full graph persistence engine.
No Ladybug integration yet.
No RuntimeGraph working set implementation beyond minimal schema/status.
No graph rebuild.
No HNSW.
No context compiler.
No memory consolidation.
No DuckDB facts.
No carrier/provider behavior changes.
No docs/manuals path updates.
```
