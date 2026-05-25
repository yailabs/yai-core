# LMDB Record Plane Freeze

Status: SPINE.34 LMDB Record Plane Freeze.

Purpose: freeze the v1 LMDB record plane before journal replay and graph
persistence work begins.

## Doctrine

LMDB is durable indexed record lookup.

```text
hot state is not LMDB
journal remains replay/audit
LMDB does not own graph truth
LMDB does not own facts
LMDB does not own memory
LMDB does not own projection authority
```

SPINE.34 freezes the current `yai.record.v1` record-plane surface. It does not
add a query planner, journal replay, graph persistence, facts, memory,
RuntimeGraph or retrieval.

## Frozen Surface

The frozen operator surface is:

```text
yai store status
yai store summary
yai store record get <record_id>
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
```

There is no journal fallback for record reads. Missing, not initialized and
unavailable LMDB environments report record-store status fields instead of
synthesizing records from journal residue.

## Frozen Indexes

SPINE.34 freezes these indexes:

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
schema_meta
```

The logical keyspace remains:

```text
record:id:<record_id>
record:case:<case_ref>:<record_id>
record:kind:<record_kind>:<record_id>
record:subject:<subject_ref>:<record_id>
record:receipt:<receipt_ref>:<record_id>
meta:schema
meta:rebuild
```

## Operational Record Coverage

The record plane must be able to preserve and query the record kinds produced
or prepared by the current control/carrier substrate:

```text
attempt
decision
receipt
receipt_requirement
carrier_request
effect_receipt
filesystem_receipt
divergence
reconciliation
```

SPINE.34 validates that `carrier_request`, `effect_receipt` and `divergence`
records can be stored under `yai.record.v1`, indexed by case/kind/subject and
linked through `records_by_receipt` when a receipt id is present.

## Boundary

SPINE.34 is a freeze, not a new data backend.

Non-goals:

```text
no journal replay implementation
no journal fallback reads
no graph persistence
no DuckDB facts
no memory consolidation
no projection delta implementation
no RuntimeGraph implementation
no HNSW implementation
no provider runtime behavior
```

## Validation

```text
make check-lmdb-record-plane-freeze
make smoke-spine34
make check
```
