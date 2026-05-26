Historical/superseded engineering record. Not an active source of truth.

# SPINE.29 LMDB Record Plane Doctrine + Schema

This is a historical archive record. The active doctrine lives in
`work/archive/architecture-snapshots/17-lmdb-record-plane.md` and
`work/archive/engineering-snapshots/lmdb-record-plane.md`.

SPINE.29 defines LMDB as the durable indexed record lookup plane under
`YAI_HOME/store/lmdb`.

Delivered:

```text
LMDB is durable indexed record lookup
hot state is not LMDB
journal remains replay/audit
schema yai.record.v1
record_env
record:id / record:case / record:kind / record:subject key grammar
yai store status
doctor record store fields
make check-lmdb-record-plane-doctrine
make smoke-spine29
```

Boundary:

```text
No LMDB write path.
No LMDB read/query path.
No journal replay implementation.
No graph backend.
No DuckDB.
No memory consolidation.
```

Next:

```text
SPINE.30 LMDB Record Write Path
```
