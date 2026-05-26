# LMDB Record Plane Freeze

Status: SPINE.34 LMDB Record Plane Freeze.

Purpose: freeze the implemented LMDB record-plane surface after SPINE.29
through SPINE.33 and before journal replay hardening begins.

## Freeze Statement

LMDB is durable indexed record lookup. journal remains replay/audit.

SPINE.34 freezes:

```text
schema = yai.record.v1
runtime path = YAI_HOME/store/lmdb
status surface = yai store status
summary surface = yai store summary
id lookup = yai store record get <record_id>
case index = yai store record list --case <case_ref>
kind index = yai store record list --kind <record_kind>
subject index = yai store record list --subject <subject_ref>
receipt index = yai store record list --receipt <receipt_ref>
```

There is no journal fallback. If LMDB is missing, not initialized or
unavailable, record read/list commands report record-store status and do not
return synthetic journal records.

## Frozen Index Set

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
schema_meta
```

Future indexes such as projection/time metadata remain planned, not part of
the SPINE.34 freeze.

## Carrier / Control Record Posture

The freeze validates that the record plane can preserve records needed by the
current carrier/control substrate:

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

The Rust store freeze test writes `carrier_request`, `effect_receipt` and
`divergence` records directly through the LMDB record-store API and verifies
case, kind, subject and receipt indexes. That proves the record plane can carry
carrier outcome / receipt / divergence material without adding new carrier
execution.

## Command Surface

Added commands:

```text
none
```

Changed commands:

```text
none
```

SPINE.34 adds no new command. It freezes existing LMDB commands and strengthens
their validation.

## Validation

```text
make check-lmdb-record-plane-doctrine
make check-lmdb-record-plane-freeze
make smoke-spine29
make smoke-spine30
make smoke-spine31
make smoke-spine32
make smoke-spine33
make smoke-spine34
make check
```

Expected smoke posture:

```text
record_store_freeze:status ok
record_store_freeze:indexes ok
record_store_freeze:record_get ok
record_store_freeze:no_journal_fallback ok
record_store_freeze:spine34 ok
```

## Next

SPINE.35 hardens Journal Replay Doctrine + Parser Hardening through journal
inspection diagnostics only. SPINE.36 replays valid journals into LMDB while
respecting the frozen `yai.record.v1` lookup and index posture.
SPINE.37 adds replay metadata, schema compatibility and cursor status without
changing LMDB query no-fallback semantics.
