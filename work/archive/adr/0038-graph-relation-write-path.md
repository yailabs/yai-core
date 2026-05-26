# ADR 0038: Graph Relation Write Path

Status: Accepted

## Context

SPINE.40 separated graph persistence from RuntimeGraph. The next step is to
materialize durable typed relations before adding a RuntimeGraph working set or
external graph persistence integration.

## Decision

YAI materializes typed graph relations from record truth before introducing
RuntimeGraph or Ladybug integration.

The v0 relation schema is `yai.graph_relation.v1`. Relations are stored in the
LMDB graph relation namespace, derived from existing LMDB records and keyed by
stable `relation_id`.

The command surface is `yai graph materialize --case <case_ref>` and
`yai graph relations --case <case_ref>`.

Initial required edge examples include `decision_controls_attempt` and
`receipt_records_effect`. Relation output preserves `relation_id` and
`source_record_id`.

RuntimeGraph remains planned. Ladybug integration remains future graph
persistence work.

## Consequences

Graph relations become inspectable and queryable by case. The write path is
idempotent and duplicate-aware. SPINE.41 does not add a graph query planner,
HNSW, Context Compiler or in-memory RuntimeGraph.
