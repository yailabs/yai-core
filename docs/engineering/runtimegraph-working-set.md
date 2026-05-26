# RuntimeGraph Working Set

Status: SPINE.42 RuntimeGraph In-Memory Working Set.

SPINE.42 adds an active-minimal RuntimeGraph working set. It loads
`yai.graph_relation.v1` relations for a case into RAM, derives runtime nodes,
runtime edges and incoming/outgoing adjacency counts, then reports the working
set without claiming resident service behavior.

RuntimeGraph is an in-memory working set. Graph persistence is durable truth.
RuntimeGraph is loaded from graph relations. HNSW remains a future candidate
index and Context Compiler remains a future consumer.
The active SPINE.42 shape is per-command ephemeral.

## Command Surface

```bash
yai graph runtime-status
yai graph runtime-load --case <case_ref>
yai graph runtime-summary --case <case_ref>
```

Required status fields:

```text
status: active_minimal
working_set: per_command_ephemeral
resident_service: planned
source: graph_relations
durable_truth: graph_persistence
hnsw: future_candidate_index
context_compiler: future_consumer
```

Required load fields:

```text
runtime_graph_load:
case_ref:
source: graph_relations
nodes_total:
edges_total:
outgoing_index_entries:
incoming_index_entries:
generation: 1
dirty: no
stale: no
durable_truth: graph_persistence
resident: false
```

## Validation

```bash
make check-runtimegraph-working-set
make smoke-spine42
```

The smoke verifies active-minimal runtime status, non-zero node and edge counts
after graph materialization, populated incoming/outgoing indexes, empty-case
zero behavior, resident service planned posture and no active HNSW.
