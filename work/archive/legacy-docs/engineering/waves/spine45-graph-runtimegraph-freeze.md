Historical/superseded engineering record. Not an active source of truth.

# SPINE.45 Graph + RuntimeGraph Freeze

Status: delivered.

SPINE.45 freezes the first graph/runtimegraph block: graph schema,
`yai.graph_relation.v1`, graph materialize, graph relations, RuntimeGraph
runtime-load/runtime-summary, runtime graph rebuild,
`yai.runtime_graph_rebuild_report.v1`, fanout, fanin, neighborhood, causal
path, bounded traversal and edge-kind filter.

RuntimeGraph is not durable truth. Plain output remains parseable and
color-aware graph inspection remains documented visual doctrine. HNSW future,
Context Compiler future and Ladybug future persistence integration remain
outside the active graph block.

Review/control records are included in the freeze. `review_request`,
`review_decision` and `control_pending` are graph-visible through minimal
relations when their actual refs exist:

```text
review_request_for_attempt
review_decision_resolves_request
control_pending_blocks_attempt
review_resolution_produces_receipt
```

Approve is visible as a review resolution path. Deny, defer and quarantine
preserve no-execution posture through SPINE.44C: `carrier_attempted: false` and
`execution_performed: false`.

Validation:

```bash
make check-graph-runtimegraph-freeze
make smoke-spine45
```
