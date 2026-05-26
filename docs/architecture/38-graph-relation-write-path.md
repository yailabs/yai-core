# Graph Relation Write Path

Status: SPINE.41 Graph Relation Write Path.

SPINE.41 starts graph relation materialization without implementing
RuntimeGraph or Ladybug integration. It stores derived typed relations from
LMDB record truth as `yai.graph_relation.v1` records in the local LMDB graph
relation namespace.

SPINE.42 loads `yai.graph_relation.v1` into an active-minimal RuntimeGraph
in-memory working set with `yai graph runtime-load --case <case_ref>`. Graph
persistence remains durable truth.

## Boundary

Graph persistence owns durable typed relations. RuntimeGraph remains planned as
the in-memory active case working set. Ladybug integration remains future graph
persistence work.

Graph relations are not vector proximity, memory, facts or projections. They
are typed relationships derived from records with `source_record_id` and
`source_record_kind` provenance.

## Schema

`yai.graph_relation.v1` contains:

```text
relation_id
case_ref
from_ref
to_ref
edge_kind
from_kind
to_kind
source_record_id
source_record_kind
confidence
created_at_unix_ms
provenance
```

Initial materialization includes:

```text
subject_participates_in_case
attempt_targets_subject
decision_controls_attempt
receipt_records_effect
policy_constrains_subject
projection_exposes_record
model_output_produces_interpretation
divergence_describes_mismatch
record_materializes_node
```

Relations are written only when the source record contains enough refs. Missing
refs are skipped; SPINE.41 does not invent fake graph edges.

## Commands

```bash
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref> --limit 20
```

`yai graph materialize` reads LMDB records for a case and writes derived
relations to `lmdb_graph_relations_v0`. The write is idempotent and reports
`relations_written`, `relations_duplicate` and `relations_skipped`.

`yai graph relations` lists relations by case and prints `relation_id`,
`edge_kind`, `from_ref`, `to_ref` and `source_record_id`.

## Non-Goals

No RuntimeGraph in-memory working set.
No Ladybug dependency or integration.
No graph query planner.
No HNSW.
No Context Compiler.
