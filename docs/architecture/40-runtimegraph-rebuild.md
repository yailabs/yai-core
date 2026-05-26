# RuntimeGraph Rebuild

Status: SPINE.43 RuntimeGraph Rebuild from Journal / LMDB / Graph Store.

SPINE.43 makes RuntimeGraph rebuild explicit. RuntimeGraph rebuild reconstructs
the per-command working set from replayable and durable sources:

```text
journal
-> LMDB
-> graph relations
-> RuntimeGraph rebuild
-> runtime-summary
```

RuntimeGraph is not durable truth. Journal remains replay/audit chronology,
LMDB remains durable record lookup and graph relations remain durable typed
relation materialization. RuntimeGraph remains per-command ephemeral and the
resident service planned.

## Report

RuntimeGraph rebuild emits `yai.runtime_graph_rebuild_report.v1` under
`YAI_HOME/store/graph/rebuild-reports/`.

Required fields include:

```text
case_ref
source_mode
journal_path
journal_identity
lmdb_path
graph_relation_source
records_seen
relations_seen
relations_written
relations_duplicate
nodes_total
edges_total
outgoing_index_entries
incoming_index_entries
runtime_generation
dirty
stale
rebuild_status
errors
warnings
```

## Commands

```bash
yai graph rebuild --case <case_ref> --from journal --path <journal.jsonl>
yai graph rebuild --case <case_ref> --from graph-relations
yai graph rebuild-report --case <case_ref>
yai graph runtime-summary --case <case_ref>
```

The journal mode runs replay to LMDB, graph materialization and RuntimeGraph
load. The graph-relations mode loads RuntimeGraph from existing graph
relations and writes the same report schema.

## Non-Goals

No resident RuntimeGraph service.
No Ladybug integration.
No HNSW.
No retrieval units.
No Context Compiler.
No legacy manual update.
No docs/labs/nvidia cleanup.

The active lab path is `docs/labs/filesystem-loop`.

## Query Boundary

SPINE.44 adds RuntimeGraph fanout, fanin, neighborhood and causal path
diagnostics after rebuild. RuntimeGraph is not a generic graph database; query
is bounded traversal over the rebuilt active-case working set. The edge-kind
filter applies to fanout, fanin and neighborhood. Path diagnostics use
max-depth. Plain output remains parseable and color-aware graph inspection is
future console/studio doctrine.

SPINE.45 freezes runtime graph rebuild with
`yai.runtime_graph_rebuild_report.v1` and validates rebuild from journal and
from graph relations. RuntimeGraph is not durable truth. Review/control
records are graph-visible when actual refs exist.
