# RuntimeGraph Rebuild

Status: SPINE.43 RuntimeGraph Rebuild from Journal / LMDB / Graph Store.

SPINE.43 adds RuntimeGraph rebuild command surface. RuntimeGraph
rebuild proves the in-memory working set can be reconstructed from replayable
and durable sources without making RuntimeGraph durable truth.

```text
journal -> LMDB -> graph relations -> runtime_graph_rebuild -> runtime-summary
```

RuntimeGraph is not durable truth. It remains per-command ephemeral; resident
service planned is the correct posture.

## Commands

```bash
yai graph rebuild --case <case_ref> --from journal --path <journal.jsonl>
yai graph rebuild --case <case_ref> --from graph-relations
yai graph rebuild-report --case <case_ref>
yai graph runtime-summary --case <case_ref>
```

Required output:

```text
runtime_graph_rebuild:
source_mode: journal|graph_relations
journal_replay_status:
graph_materialize_status:
runtime_graph_status:
nodes_total:
edges_total:
rebuild_status:
report_schema: yai.runtime_graph_rebuild_report.v1
```

The rebuild report is stored under:

```text
YAI_HOME/store/graph/rebuild-reports/
```

## Validation

```bash
make check-runtimegraph-rebuild
make smoke-spine43
```

The smoke covers journal rebuild, graph-relations rebuild, report readback,
duplicate-aware second rebuild, missing case behavior, bad journal failure and
resident service planned posture.

The active lab path is `docs/labs/filesystem-loop`.

## SPINE.44 Query Link

After rebuild, `yai graph fanout`, `yai graph fanin`, `yai graph
neighborhood` and `yai graph path --max-depth 4` inspect the active-case
RuntimeGraph working set. RuntimeGraph is not a generic graph database. These
commands provide bounded traversal diagnostics, causal path inspection,
edge-kind filter support for fanout/fanin/neighborhood, parseable plain output
and future color-aware graph inspection semantics.

SPINE.45 freezes runtime graph rebuild and
`yai.runtime_graph_rebuild_report.v1` for journal and graph-relations rebuild
sources. Review/control records are graph-visible when actual refs exist.
