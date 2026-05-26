# ADR 0044: Graph + RuntimeGraph Freeze

Status: accepted.

## Context

SPINE.40-44 introduced graph persistence, `yai.graph_relation.v1`, the
per-command RuntimeGraph working set, runtime graph rebuild reports and bounded
diagnostic queries. SPINE.44A-C introduced the operator review loop and aligned
its CLI/lab/test matrix.

## Decision

The first graph block is frozen with graph schema, relation materialization,
RuntimeGraph working set, RuntimeGraph rebuild reports and bounded diagnostic
queries. RuntimeGraph remains ephemeral and not durable truth.

The freeze includes review/control visibility. `review_request`,
`review_decision` and `control_pending` records are graph-visible through
minimal typed relations when their source refs exist. Approve is visible as a
review resolution path; deny, defer and quarantine preserve no-execution
posture in records.

## Boundaries

`yai.graph_relation.v1`, graph materialize, graph relations, runtime-load,
runtime-summary, runtime graph rebuild, `yai.runtime_graph_rebuild_report.v1`,
fanout, fanin, neighborhood, causal path, bounded traversal and edge-kind
filter are frozen diagnostic surfaces. Plain output remains parseable.
Color-aware graph inspection remains a visual doctrine.

RuntimeGraph is not durable truth. HNSW future, Context Compiler future and
Ladybug future persistence integration remain non-goals for this freeze.
