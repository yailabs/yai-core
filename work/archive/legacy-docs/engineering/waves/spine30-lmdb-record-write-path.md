Historical/superseded engineering record. Not an active source of truth.

# SPINE.30 LMDB Record Write Path

SPINE.30 adds the first LMDB record write path while preserving journal as
append-only replay/audit source.

Delivered:

```text
Rust engine LMDB dependency via lmdb crate
YAI_HOME/store/lmdb opens as record_env
schema_meta/meta:schema = yai.record.v1
records_by_id
records_by_case
records_by_kind
yai store status ready after schema initialization
yai store summary
daemon loop journal mirroring into LMDB
make smoke-spine30
```

Boundary:

```text
No record get/list read API.
No journal replay implementation.
No graph backend.
No DuckDB.
No memory consolidation.
No projection delta.
No yai-dev mutation.
```

Next:

```text
SPINE.31 LMDB Record Read / Query Path
```
