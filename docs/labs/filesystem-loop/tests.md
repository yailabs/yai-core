# Filesystem Loop Lab Tests

Status: canonical lab test expectation map.

## Journal Replay Checks

```bash
yai journal inspect --path "$JOURNAL"
yai journal replay --path "$JOURNAL" --dry-run
yai journal replay --path "$JOURNAL"
yai journal replay-status --path "$JOURNAL"
yai journal replay-report --path "$JOURNAL"
yai store summary
yai store record list --case case:new12-filesystem --limit 10
```

Expected signals include `replay_ready: yes`, `record_schema:
yai.record.v1`, `compatibility: compatible`, `cursor_line:`,
`replay_status: completed`, `replay_report_schema: yai.replay_report.v1`,
`records_written:`, `records_duplicate:`, `invalid_entries: 0` and
`idempotent: yes`.

Negative checks must keep `schema_mismatch`, `invalid_json`, no false complete
and no journal fallback visible.

## Graph Relation Write Path Checks

SPINE.41 checks graph relation materialization from replayed LMDB records.
RuntimeGraph remains planned at the SPINE.41 boundary. Ladybug integration remains future.

```bash
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref> --limit 20
```

Expected signals include `schema: yai.graph_relation.v1`,
`relations_written: N`, duplicate-aware second materialization,
`edge_kind: decision_controls_attempt`, `edge_kind: receipt_records_effect`,
`relation_id:`, `source_record_id:` and `runtime_graph_updated: false`.

## RuntimeGraph Working Set Checks

SPINE.42 checks RuntimeGraph as a per-command ephemeral in-memory working set.
The resident service remains planned, graph persistence is durable truth, HNSW
future candidate index work remains future and Context Compiler future consumer
work remains future.

```bash
yai graph schema
yai graph runtime-status
yai graph runtime-load --case <case_ref>
yai graph runtime-load --case case:missing
```

Expected signals:

```text
graph_schema:
node_kinds:
edge_kinds:
relation_write_path: active_minimal
runtime_graph:
status: active_minimal
role: in_memory_active_case_working_set
working_set: per_command_ephemeral
resident_service: planned
source: graph_relations
durable_truth: graph_persistence
nodes_total: N
edges_total: N
outgoing_index_entries:
incoming_index_entries:
dirty: no
stale: no
resident: false
case:missing nodes_total: 0
case:missing edges_total: 0
hnsw: future_candidate_index
context_compiler: future_consumer
```

## RuntimeGraph Rebuild Checks

SPINE.43 checks RuntimeGraph rebuild from replayable and durable sources.
RuntimeGraph is not durable truth; the active lab path is
`docs/labs/filesystem-loop`.

```bash
yai graph rebuild --case <case_ref> --from journal --path <journal.jsonl>
yai graph rebuild-report --case <case_ref>
yai graph runtime-summary --case <case_ref>
yai graph rebuild --case <case_ref> --from graph-relations
```

Expected signals include `RuntimeGraph rebuild`, `runtime_graph_rebuild`,
`report_schema: yai.runtime_graph_rebuild_report.v1`, `journal`, `LMDB`,
`graph relations`, `runtime-summary`, `resident service planned`, completed
journal/materialize/runtime statuses, missing-case zero nodes/edges and bad
journal failure without a false complete.
