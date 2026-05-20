# Store, Index And Graph Model

The data plane is split into separate roots because persistence, retrieval and reconstruction are different responsibilities.

## Store

Store owns physical persistence posture:

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

Records are facts, store is persistence, graph is relation, index is retrieval, memory is continuity, projection is read model.
