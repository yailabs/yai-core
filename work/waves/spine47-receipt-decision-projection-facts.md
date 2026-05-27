# SPINE.47 - Receipt / Decision / Projection Facts

Status: complete.

## Purpose

Implement the first real fact extraction from LMDB records into DuckDB.

This wave extracts:

```text
fact_receipt
fact_decision
fact_projection
```

Facts are not truth. Fact extraction is derivation, not migration. Records
remain truth in LMDB.

## Command Surface

```bash
yai facts extract --case <case_ref> --kind receipt
yai facts extract --case <case_ref> --kind decision
yai facts extract --case <case_ref> --kind projection
yai facts extract --case <case_ref> --kind core
yai facts summary --case <case_ref>
```

`core` means receipt + decision + projection.

## Derivation Rules

Receipt facts derive from receipt-like record kinds:

```text
receipt
filesystem_receipt
effect_receipt
carrier_receipt
process_receipt
```

Decision facts derive from:

```text
decision
review_decision
```

Projection facts derive from:

```text
projection_result
projection_request
participant_view_frame
```

Fields are extracted from structured record fields first and from conservative
summary tokens where present. Missing values use empty string, false or zero
according to column type.

## Bitemporal Posture

For SPINE.47:

```text
transaction_time = extraction time
known_at = extraction time
valid_time_start = source record created_at_unix_ms when available, else extraction time
valid_time_end sentinel: 0
status = current
revision_of = ""
superseded_by = ""
retracted_by = ""
confidence = 1.0
fact_schema = yai.fact.v1
```

Revision fields are present but empty. No fact revision or supersession is
implemented in this wave.

## Idempotency

Fact IDs are deterministic fact IDs:

```text
fact:<kind>:<source_record_id>
```

Extraction is idempotent extraction. A second run reports `facts_duplicate`
instead of inserting duplicate rows.

## Validation

```bash
make check-receipt-decision-projection-facts
make smoke-spine47
```

Expected smoke labels:

```text
facts_extract:receipt ok
facts_extract:decision ok
facts_extract:projection ok
facts_extract:core ok
facts_extract:idempotent ok
facts_summary:counts ok
facts:not_truth ok
```

## Non-Goals

No model behavior facts. No policy outcome facts. No memory quality facts. No
retrieval quality facts. No provider runtime facts. No fact revision. No graph
patching. No report generation beyond `facts summary`.
