# Graph Persistence / RuntimeGraph

Status: SPINE.40 Graph Persistence / RuntimeGraph Doctrine + Schema.

## Purpose

SPINE.40 starts the graph block after the journal replay freeze. It separates
durable relation truth from active in-memory graph computation.

Canonical rule:

```text
Persistent truth on disk.
Computational shape in memory.
Vector proximity as candidate discovery.
Compiled projection as model-facing view.
```

## Boundary

```text
LMDB stores records.
Graph persistence stores typed relations.
RuntimeGraph computes over the active case.
HNSW finds candidate runtime nodes.
Context Compiler renders controlled views.
```

HNSW finds candidate nodes. HNSW is not graph truth. RuntimeGraph is the
in-memory active case working set. Projection does not disappear.

Graph persistence owns durable typed relations, causal edges, node/edge
persistence posture, rebuild from journal/LMDB, graph auditability and future
migration/compatibility. It is not a runtime working set, vector search, memory,
facts plane or model context.

RuntimeGraph owns in-memory active-case computation: node/edge working set,
adjacency indexes, reverse adjacency indexes, case/subject/policy indexes,
bounded traversal, path expansion, future retrieval-unit bindings, dirty delta
overlay and generation/staleness counters. It is not durable truth, audit log,
graph persistence, HNSW, LMDB or DuckDB.

## Schema

SPINE.40 defines initial graph node kinds:

```text
case
subject
operation
attempt
decision
dispatch
carrier
receipt
effect
observation
divergence
policy
projection
memory_candidate
model_output
model_interpretation
record
unknown
```

Initial edge kinds:

```text
belongs_to_case
subject_participates_in_case
attempt_targets_subject
decision_controls_attempt
dispatch_routes_decision
carrier_realizes_dispatch
receipt_records_effect
observation_checks_effect
divergence_describes_mismatch
policy_constrains_subject
policy_constrains_operation
projection_exposes_record
model_output_produces_interpretation
memory_derived_from_receipt
record_materializes_node
derived_from
supports
contradicts
unknown
```

## CLI

```bash
yai graph schema
yai graph runtime-status
```

`yai graph schema` prints the node and edge schema plus a no-store claim.
`yai graph runtime-status` says RuntimeGraph is planned and identifies
`graph_persistence`, `future_candidate_index` and `future_consumer` boundaries.

## Non-Goals

```text
No full graph persistence engine.
No Ladybug integration.
No RuntimeGraph working set beyond schema/status.
No graph rebuild.
No HNSW.
No context compiler.
No DuckDB facts.
No memory consolidation.
```
