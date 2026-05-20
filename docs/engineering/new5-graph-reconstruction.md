# NEW.5 Graph Reconstruction

NEW.5 adds the first operational graph layer over persisted records. It does not
introduce a graph database, advanced traversal or a Rust production graph
backend.

## Implemented Surface

```text
graph edge record v0
canonical edge kinds
graph edges persisted through journal records
receipt chain reconstruction
projection graph counts
yaictl graph summary
```

## Edge Kinds

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

## Record Kinds

NEW.5 adds:

```text
graph_edge
reconstruction
```

The current JSONL shape remains the generic store record envelope. Edge details
are encoded in the record summary:

```text
edge:<edge_kind> from:<ref> to:<ref>
```

## Reconstruction V0

The first reconstruction path answers why a receipt exists:

```text
receipt_id
case_id
attempt_id
decision_id
subject_id
edge_count
```

This is a compact receipt-chain proof, not a general traversal engine.

## Debug Command

```text
crates/target/debug/yaictl graph summary --journal build/tmp/new5/journal.jsonl
```

## Out Of Scope

```text
no graph database
no advanced traversal
no visual graph
no cross-case graph
no cycle detection
no query planner
no Rust production graph backend
```

## Validation

```text
make smoke-new5
make smoke
make check
cargo fmt --check
git diff --check
```
