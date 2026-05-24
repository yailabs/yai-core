# Store, Index And Graph Model

The data plane is split because hot state, replay, durable records, retrieval,
facts, reconstruction, memory, projection and mismatch detection are different
responsibilities. SPINE.3R makes the mature owner explicit: Rust owns the
operational data planes under target `engine/`, while C keeps the host, daemon,
carrier, control and ABI boundary under target `system/`.

Doctrine:

```text
YAI is residue-first.
Store is not memory.
Graph is not lineage.
Index/query is not RAG.
Memory is not chat history.
Projection is not UI state.
Reconcile is not recovery execution.
```

SPINE.3R stratifies the operational data planes:

| Plane | Target role |
|---|---|
| hot | shared memory / hot state, not truth |
| journal | append-only replay chronology |
| record | LMDB or equivalent durable KV lookup |
| graph | Ladybug operational graph |
| fact | DuckDB derived analytical facts |
| memory | operational continuity over residue |
| projection | live, versioned, delta-aware view materialization |
| reconcile | expected-vs-observed mismatch detection |
| observability/evaluation | trace, timing, freshness, model behavior and quality diagnostics |

## Store

Store owns durable persistence posture:

```text
journal
record store
receipt store
source store
blob store
retention
integrity
backend abstraction
```

Store does not own semantic truth.

SPINE.4 adds diagnostic residue pressure: future store backends must support
trace lookup, diagnostic state lookup and case-view version lookup without
turning diagnostics into source truth.

Target owner: Rust engine.

NEW.13 route:

```text
system/store -> split: system/engine_bridge + engine/yai-engine/src/store
```

NEW.14 moved the Rust engine crate that owns the future store path to
`engine/yai-engine`. NEW.17 moved C store code to `system/store`; it remains
transitional there after NEW.18. The Rust engine C shim now lives in
`system/engine_bridge/rust_engine_backend.c`.

## NEW.2 Store V0

The first persistent store path is intentionally narrow:

```text
build/tmp/new2/journal.jsonl
```

Each line is a `yai.store.record.v0` JSON object produced by the C record codec
and loaded back into the C in-memory journal. This is not yet the stable user
layout, not a multi-case store and not the Rust production backend.

NEW.2 store guarantees:

```text
append-only file journal
record serialization/deserialization for known v0 fields
receipt linkage through attempt_id, decision_id and receipt_id
projection derived from reloaded records
subject state reconstructed from records
```

NEW.2 explicitly does not implement locking, encryption, indexing, graph
traversal or memory consolidation.

## NEW.6 Memory Candidate Records

NEW.6 adds a residue-derived record kind:

```text
memory_candidate
```

The record keeps the same `yai.store.record.v0` envelope and uses existing refs:

```text
case_ref
subject_ref
attempt_id
decision_id
receipt_id
summary
```

The summary carries candidate posture and basis counts. Store persists the fact;
memory owns continuity semantics; projection owns read-model counts.

## NEW.7 Divergence And Reconciliation Records

NEW.7 adds residue-derived record kinds:

```text
divergence
reconciliation
```

The records keep the same `yai.store.record.v0` envelope. Divergence records
link to attempt, decision and receipt refs where available. Reconciliation
records record posture such as `requires_review`; they do not execute recovery.

## NEW.8 Projection Request And Result Records

NEW.8 adds controlled read-model record kinds:

```text
projection_request
projection_result
```

They keep the same `yai.store.record.v0` envelope. The request records consumer
and projection kind. The result records provenance counts and posture in the
summary, including redaction and freshness. Store persists the view artifact;
projection owns read-model shaping; records remain the source residue.

## Index

Index owns technical retrieval structures:

```text
lexical index
structured index
semantic index
vector index
hybrid retrieval
freshness
query safety
```

Index does not own memory or projection truth.

Target owner: Rust engine.

NEW.13 route:

```text
system/index -> split: system/engine_bridge + engine/yai-engine/src/index/query
```

NEW.14 moved the Rust engine crate that owns the future index/query path to
`engine/yai-engine`. NEW.17 moved C index/query code to `system/index`; it
remains transitional there after NEW.18 as `keep_temporarily`.

## NEW.9 Query Boundary V0

NEW.9 adds a minimal query boundary under `index`:

```text
query request
query filter
query result
journal scan
```

It supports optional filters by case, record kind, subject, decision id, receipt
id, memory marker and projection marker. Results remain in journal order and
can be limited. This is deliberately not a database, index file, graph
traversal, vector search or retrieval ranking layer.

## Observability Inputs

Store, index and graph feed the Operational Observability & Evaluation Plane:

```text
journal trace records
timing samples
diagnostic records
record replayability
trace lookup
case-view version lookup
graph_chain_completeness
causal reconstruction
orphan edge detection
```

SPINE.4 documents future records such as `trace`, `trace_span`,
`timing_sample`, `rebuild_report` and `analytics_fact`; it does not implement
them.

`query_result` is a derived residue record kind for count summaries. Store
persists the result artifact; query owns scan/filter posture; projection may
summarize query results but does not become query truth.

## NEW.10 Rust Engine R1

NEW.10 adds a Rust operational data engine mirror behind the C ABI. It consumes
the same `yai.store.record.v0` JSONL residue and can append records, count by
kind, query by kind and build projection summary JSON.

This is not a store backend switch. The C file-backed journal remains active
and tested. Rust R1 is an internal backend path for operational data mechanics,
not semantic ownership.

## SPINE.3R Backend Direction

```text
journal plane = replay
record plane = LMDB or equivalent durable normalized lookup
graph plane = Ladybug persistent operational graph
fact plane = DuckDB derived analytical facts
```

These backends are planned-not-created in SPINE.3R. The current JSONL journal
and C smoke paths remain bootstrap behavior until their dedicated data-plane
waves.

## Graph

Graph owns causal reconstruction and relationship edges:

```text
subject graph
operation graph
decision graph
receipt graph
policy graph
memory graph
causal reconstruction
```

Graph is the replacement for final `lineage` semantics.

Target owner: Rust engine.

NEW.13 route:

```text
system/graph -> split: system/engine_bridge + engine/yai-engine/src/graph
```

NEW.14 moved the Rust engine crate that owns the future graph path to
`engine/yai-engine`. NEW.17 moved C graph code to `system/graph`; it remains
transitional there after NEW.18 as `keep_temporarily`.

## NEW.5 Graph V0

NEW.5 stores explicit graph edges as journal records:

```text
graph_edge
reconstruction
```

Initial edge kinds:

```text
case_binds_subject
op_targets_subject
op_has_actor
decision_controls_op
gate_supports_decision
obligation_attached_to_decision
receipt_records_effect
receipt_updates_subject
carrier_acts_on_subject
projection_derived_from_record
```

The first reconstruction path builds a receipt chain:

```text
case -> op -> decision -> receipt -> subject
```

This is not a graph database or traversal planner. It is a persisted
relationship layer over store records so the core can explain why a receipt
exists and which case, operation, decision and subject it belongs to.

## Split Rule

Records are facts, store is persistence, graph is relation, index/query is
operational access, memory is receipt-backed continuity, projection is a
controlled read model and reconcile is mismatch posture. C bootstrap data logic
is transitional and will be thinned as Rust data-spine ownership matures.
