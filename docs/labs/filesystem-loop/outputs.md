# Filesystem Loop Outputs Compatibility Pointer

Status: compatibility pointer.

The compact output surface is now run-local:

```text
docs/labs/filesystem-loop/runs/YYYYMMDD-<slug>/
  run.md
  report.md
  manifest.json
  metrics.json
  assets/
```

Aggregate output is test-local:

```text
docs/labs/filesystem-loop/analytics/
```

This file remains only for repository guards that still check the historical
`outputs.md` path.

## Graph Relation Write Path Output

SPINE.41 graph relation output is active-minimal relation materialization.
RuntimeGraph remains planned at the SPINE.41 boundary. Ladybug integration remains future.

```bash
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref> --limit 20
```

Expected fields include `schema: yai.graph_relation.v1`,
`relation_id:`, `source_record_id:`, `edge_kind: decision_controls_attempt`
and `edge_kind: receipt_records_effect`.

## RuntimeGraph Working Set Output

SPINE.42 RuntimeGraph output is an active-minimal per-command ephemeral
in-memory working set loaded from graph relations. Graph persistence is durable
truth, HNSW future candidate index work remains future and Context Compiler
future consumer work remains future.

```bash
yai graph runtime-status
yai graph runtime-load --case <case_ref>
yai graph runtime-load --case case:missing
```

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

runtime_graph_load:
nodes_total:
edges_total:
outgoing_index_entries:
incoming_index_entries:
dirty: no
stale: no
resident: false
```

## RuntimeGraph Rebuild Output

SPINE.43 output reports RuntimeGraph rebuild through
`yai.runtime_graph_rebuild_report.v1`. RuntimeGraph is not durable truth; the
journal to LMDB to graph relations chain stays explicit. The active lab path is
`docs/labs/filesystem-loop`.

```text
runtime_graph_rebuild:
source_mode: journal
journal_replay_status: completed
graph_materialize_status: completed
runtime_graph_status: completed
nodes_total: <non-zero>
edges_total: <non-zero>
rebuild_status: completed
report_schema: yai.runtime_graph_rebuild_report.v1
```

The graph relations path must include `source_mode: graph_relations`, graph
relations counts, `runtime-summary`, and resident service planned. Bad journals
must report `journal_replay_status: failed`, `rebuild_status: failed`, zero
nodes/edges and the specific error such as `invalid_json`.
