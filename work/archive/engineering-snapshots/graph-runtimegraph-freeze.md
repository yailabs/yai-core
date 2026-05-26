# Graph + RuntimeGraph Freeze

Status: SPINE.45 freeze.

SPINE.45 freezes the graph/runtimegraph block across command surface, smoke
tests and lab docs. The frozen lifecycle is:

```bash
yai graph schema
yai graph runtime-status
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref>
yai graph runtime-load --case <case_ref>
yai graph runtime-summary --case <case_ref>
yai graph rebuild --case <case_ref> --from journal --path <journal.jsonl>
yai graph rebuild --case <case_ref> --from graph-relations
yai graph rebuild-report --case <case_ref>
yai graph fanout --case <case_ref> --node <ref>
yai graph fanin --case <case_ref> --node <ref>
yai graph neighborhood --case <case_ref> --node <ref>
yai graph path --case <case_ref> --from <ref> --to <ref>
```

The frozen record/graph schemas include `yai.graph_relation.v1` and
`yai.runtime_graph_rebuild_report.v1`. RuntimeGraph is not durable truth:
durable truth remains journal plus LMDB records plus graph relations.

Diagnostic query boundaries:

```text
fanout
fanin
neighborhood
causal path
bounded traversal
edge-kind filter
plain output remains parseable
color-aware graph inspection
```

Future boundaries remain explicit: HNSW future, Context Compiler future and
Ladybug future persistence integration.

Review/control visibility is frozen too. The materializer emits
`review_request_for_attempt`, `review_decision_resolves_request`,
`control_pending_blocks_attempt` and `review_resolution_produces_receipt` when
the source records contain actual refs. This covers `review_request`,
`review_decision`, `control_pending` and the approve path. Deny, defer and
quarantine keep `carrier_attempted: false` and `execution_performed: false`
through SPINE.44C no-execution records.

Validation:

```bash
make check-graph-runtimegraph-freeze
make smoke-spine45
```
