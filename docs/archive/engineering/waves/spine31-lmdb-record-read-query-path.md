Historical/superseded engineering record. Not an active source of truth.

# SPINE.31 LMDB Record Read / Query Path

SPINE.31 makes the LMDB record plane readable from the CLI after SPINE.30
introduced the write path.

Delivered:

```text
yai store record get <record_id>
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
```

The read path uses:

```text
records_by_id
records_by_case -> records_by_id
records_by_kind -> records_by_id
```

Boundary:

```text
journal remains replay/audit
LMDB remains durable indexed record lookup
hot state remains live cache/freshness
missing LMDB does not fake reads
missing record returns record: not_found
```

Validation surface:

```text
make smoke-spine31
target/debug/yai store record list --kind receipt --limit 10
target/debug/yai store record list --case case:new12-daemon --limit 10
target/debug/yai store record get rec:new12-min-receipt
target/debug/yai store record get rec:missing
```

Non-goals:

```text
No new write semantics.
No journal replay implementation.
No subject, receipt, projection or time indexes.
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
SPINE.32 LMDB Case / Subject / Receipt Indexes
```
