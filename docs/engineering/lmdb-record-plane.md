# LMDB Record Plane

SPINE.29 freezes the record-plane contract before SPINE.30 implements writes.

## Operator Doctrine

```text
LMDB is durable indexed record lookup.
hot state is not LMDB.
journal remains replay/audit.
```

The record plane lives under:

```text
YAI_HOME/store/lmdb
```

`yai store status` is the SPINE.29 command because `store` is already the
runtime durable data root and LMDB is the record-plane backend under it. Future
commands can extend this shape:

```text
yai store status
yai store record get <record_id>
yai store record list --case <case_ref>
yai store rebuild status
```

SPINE.29 implements only `yai store status`.

## Status Surface

Fields:

```text
record_store_backend: lmdb
record_store_status: missing|not_initialized|unavailable
record_store_path: <YAI_HOME>/store/lmdb
record_env: record_env
schema: yai.record.v1
```

`missing` means the directory is absent. `not_initialized` means the directory
exists but SPINE.29 has not created LMDB files or schema metadata. `unavailable`
means the path exists but is not a directory. `ready` is intentionally not
reported in SPINE.29.

`yai doctor` includes the record store fields beside the hot-state diagnostics.

## Schema And Keyspace

The v1 logical schema is `yai.record.v1`. Logical keys include:

```text
record:id:<record_id>
record:case:<case_ref>:<record_id>
record:kind:<kind>:<record_id>
record:subject:<subject_ref>:<record_id>
record:receipt:<receipt_ref>:<record_id>
record:projection:<projection_ref>:<record_id>
record:time:<unix_ms>:<record_id>
meta:schema
meta:rebuild
```

The required value envelope fields are:

```text
schema
record_id
record_kind
case_ref
subject_refs
source
created_at_unix_ms
provenance_refs
payload
```

Minimum envelope:

```json
{
  "schema": "yai.record.v1",
  "record_id": "rec:...",
  "record_kind": "...",
  "case_ref": "case:...",
  "created_at_unix_ms": 0,
  "source": {
    "plane": "journal",
    "ref": "..."
  },
  "payload": {}
}
```

## Planned Indexes

SPINE.30-32 will build these indexes incrementally:

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
records_by_projection
records_by_time
record_meta
schema_meta
```

They are rebuildable from journal residue. LMDB must not replace journal replay
or audit.

## yai-dev Audit

SPINE.29 read-only inspected:

```text
../yai-dev/src/substrate/store/*
../yai-dev/src/substrate/records/*
../yai-dev/src/substrate/query/*
../yai-dev/src/substrate/views/*
../yai-dev/src/case/evidence/*
../yai-dev/src/runtime/execution/execution_records.c
../yai-dev/src/runtime/execution/operational_receipt.c
```

Concepts retained: record lifecycle, taxonomy, store backend posture, LMDB as a
store boundary, filesystem store as debug/backing residue, record relationship
boundaries, query surfaces, receipt persistence and evidence refs.

Concepts rejected: making DuckDB the record store, making views canonical
truth, making graph edges own record persistence, and mutating `yai-dev`.

## Validation

SPINE.29 guards:

```text
make check-lmdb-record-plane-doctrine
make smoke-spine29
```

They are integrated into:

```text
make check-docs
make smoke
make check
```
