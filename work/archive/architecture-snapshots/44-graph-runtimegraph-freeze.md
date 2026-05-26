# Graph + RuntimeGraph Freeze

Status: SPINE.45 freeze.

SPINE.45 freezes the first graph/runtimegraph block before DuckDB facts. The
frozen lifecycle is graph schema, `yai.graph_relation.v1`, graph materialize,
graph relations, RuntimeGraph `runtime-load`, RuntimeGraph `runtime-summary`,
runtime graph rebuild, `yai.runtime_graph_rebuild_report.v1`, fanout, fanin,
neighborhood and causal path diagnostics.

RuntimeGraph is not durable truth. Graph persistence stores durable typed
relations; RuntimeGraph computes over active-case graph data in a per-command
ephemeral working set. Bounded traversal remains diagnostic only: fanout,
fanin, neighborhood, causal path and edge-kind filter are not a generic graph
database language.

Freeze boundary:

```text
RuntimeGraph is not durable truth
plain output remains parseable
color-aware graph inspection
HNSW future
Context Compiler future
Ladybug future persistence integration
```

Review/control coverage is part of the freeze. The materializer maps existing
review/control refs with minimal typed relations:

```text
review_request_for_attempt
review_decision_resolves_request
control_pending_blocks_attempt
review_resolution_produces_receipt
```

These relations make `review_request`, `review_decision` and `control_pending`
graph-visible without inventing refs. The approve path is graph/query visible.
Deny, defer and quarantine keep their no-execution posture in records and are
covered by SPINE.44C: `carrier_attempted: false` and
`execution_performed: false`.
