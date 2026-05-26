# Graph Relation Write Path

Status: SPINE.41 Graph Relation Write Path.

SPINE.41 makes graph relations explicit and inspectable by materializing
`yai.graph_relation.v1` from existing LMDB records. RuntimeGraph remains planned.
Ladybug integration remains future.

SPINE.42 loads those graph relations into an active-minimal RuntimeGraph
in-memory working set. Graph persistence remains durable truth.

## Command Surface

```bash
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref> --limit 20
```

Materialization output:

```text
graph_materialize:
case_ref: case:new12-filesystem
source: lmdb_records
relations_seen: N
relations_written: N
relations_duplicate: N
relations_skipped: N
schema: yai.graph_relation.v1
graph_store: lmdb_graph_relations_v0
runtime_graph_updated: false
```

Relation list output:

```text
graph_relations:
case_ref: case:new12-filesystem
relations_total: N
limit: 20
relations:
- relation_id: ...
  edge_kind: decision_controls_attempt
  from_ref: decision:new12-fs-write
  to_ref: attempt:new12-fs-write
  source_record_id: rec:new12-fs-write-decision
```

## Derived Relations

SPINE.41 derives relations only from record fields already present in
`yai.record.v1`. It can derive `decision_controls_attempt`,
`receipt_records_effect`, `policy_constrains_subject`,
`divergence_describes_mismatch`, `projection_exposes_record`,
`model_output_produces_interpretation`, `attempt_targets_subject`,
`subject_participates_in_case` and `record_materializes_node`.

Every relation includes `relation_id`, `source_record_id` and
`source_record_kind`. Duplicate relation ids are counted as duplicates, not
rewritten.

## Validation

```bash
make check-graph-relation-write-path
make smoke-spine41
```

The smoke verifies graph materialization from LMDB records, idempotent second
materialization, relation listing, `decision_controls_attempt`,
`receipt_records_effect` and `runtime_graph_updated: false`.
