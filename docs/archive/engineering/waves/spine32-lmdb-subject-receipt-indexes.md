Historical/superseded engineering record. Not an active source of truth.

# SPINE.32 LMDB Case / Subject / Receipt Indexes

SPINE.32 adds subject and receipt secondary indexes to the LMDB record plane.

Delivered:

```text
records_by_subject
records_by_receipt
record:subject:<subject_ref>:<record_id> -> record_id
record:receipt:<receipt_ref>:<record_id> -> record_id
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
records_by_subject summary count
records_by_receipt summary count
```

Boundary:

```text
indexes are derived lookup views
indexes are not new truth
journal remains replay/audit
LMDB remains durable indexed record lookup
no free-text payload parsing
no journal fallback reads
```

Validation surface:

```text
make smoke-spine32
target/debug/yai store record list --subject subject:filesystem-sandbox --limit 20
target/debug/yai store record list --receipt receipt:new12-fs-write --limit 10
target/debug/yai store record list --subject subject:missing --limit 10
```

Non-goals:

```text
No projection index.
No time index.
No journal replay implementation.
No graph backend.
No DuckDB.
No memory consolidation.
No projection delta.
No model/provider work.
No carrier expansion.
No interfaces or Studio changes.
No yai-dev mutation.
```

Next:

```text
SPINE.33 LMDB CLI + Manual Validation
```
