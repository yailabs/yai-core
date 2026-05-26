# RuntimeGraph Working Set

Status: SPINE.42 RuntimeGraph In-Memory Working Set.

SPINE.42 introduces RuntimeGraph as a per-command ephemeral in-memory working
set loaded from `yai.graph_relation.v1` graph relations for one case.

RuntimeGraph is not graph truth. RuntimeGraph is not Ladybug. RuntimeGraph is
not HNSW. RuntimeGraph computes over the active case working set while graph
persistence remains durable truth.

## Boundary

```text
graph persistence = durable typed relations
RuntimeGraph = in-memory working set
HNSW = future candidate index
Context Compiler = future consumer
```

The first working set contains runtime nodes, runtime edges, outgoing adjacency
counts and incoming adjacency counts. It is loaded on demand by CLI commands
and is not a resident service yet.

## Runtime Node

```text
node_ref
node_kind
case_ref
source_record_ref
generation
```

## Runtime Edge

```text
relation_id
edge_kind
from_ref
to_ref
case_ref
source_record_id
generation
```

## Commands

```bash
yai graph runtime-status
yai graph runtime-load --case <case_ref>
yai graph runtime-summary --case <case_ref>
```

`yai graph runtime-status` reports `status: active_minimal`,
`working_set: per_command_ephemeral`, `resident_service: planned`,
`source: graph_relations`, `durable_truth: graph_persistence`,
`hnsw: future_candidate_index` and `context_compiler: future_consumer`.

`yai graph runtime-load` loads graph relations for a case, deduplicates nodes,
builds edge and adjacency counts and reports `resident: false`.

## Non-Goals

No resident RuntimeGraph service.
No Ladybug integration.
No HNSW.
No retrieval units.
No Context Compiler.
No graph query planner.
