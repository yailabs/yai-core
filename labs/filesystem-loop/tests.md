# Filesystem Loop Tests

Status: canonical lab test matrix.

## Fact Plane Extraction

SPINE.47 tests the first real fact extraction from LMDB records into DuckDB:

```bash
yai facts init
yai facts extract --case case:new12-filesystem --kind receipt
yai facts extract --case case:new12-filesystem --kind decision
yai facts extract --case case:new12-filesystem --kind projection
yai facts extract --case case:new12-filesystem --kind core
yai facts summary --case case:new12-filesystem
```

Expected posture:

```text
fact_receipt > 0
fact_decision > 0
fact_projection > 0
facts_are_truth: false
```

Facts are not truth. Fact extraction is derivation, not migration. Records
remain truth in LMDB. Deterministic fact IDs use:

```text
fact:<kind>:<source_record_id>
```

Second extraction must be duplicate-aware idempotent extraction and report
`facts_duplicate` instead of inserting duplicate rows.

Bitemporal fields are populated:

```text
transaction_time
valid_time_start
valid_time_end
known_at
```

Revision fields remain present but empty until future revision work:

```text
revision_of
superseded_by
retracted_by
```

valid_time_end sentinel: 0 means open-ended. No fact revision is implemented in
SPINE.47.
