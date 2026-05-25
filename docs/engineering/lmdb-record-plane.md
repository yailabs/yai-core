# LMDB Record Plane

SPINE.29 freezes the record-plane contract. SPINE.30 implements the first LMDB
write path. SPINE.31 adds the first read/query path over the id, case and kind
indexes. SPINE.32 adds derived subject and receipt indexes. SPINE.33 freezes
the CLI output shape and manual validation matrix. SPINE.34 freezes the record
plane before journal replay work begins. SPINE.35 adds journal replay parser
inspection without writing LMDB. SPINE.36 adds journal replay to LMDB through
the frozen record/index writer.

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

`yai store status` is the readiness command because `store` is already the
runtime durable data root and LMDB is the record-plane backend under it.
SPINE.30 adds aggregate summary. SPINE.31 adds record reads:

```text
yai store status
yai store summary
yai store record get <record_id>
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
```

Future commands can extend this shape:

```text
yai store rebuild status
```

## Status Surface

Fields:

```text
record_store_backend: lmdb
record_store_status: missing|not_initialized|ready|unavailable
record_store_path: <YAI_HOME>/store/lmdb
record_env: record_env
schema: yai.record.v1
```

`missing` means the directory is absent. `not_initialized` means the directory
exists but no schema metadata exists. `unavailable` means the path exists but is
not a directory or cannot be opened. `ready` means the LMDB environment opens
and `schema_meta/meta:schema` is `yai.record.v1`.

`yai doctor` includes the record store fields beside the hot-state diagnostics.

`yai store summary` reports:

```text
record_store_backend: lmdb
record_store_status: ready
record_store_path: <YAI_HOME>/store/lmdb
records_total: N
records_by_case: N
records_by_kind: N
records_by_subject: N
records_by_receipt: N
```

`yai store record get <record_id>` reads `records_by_id` and prints the
`yai.record.v1` envelope fields plus a compact payload summary. A missing
record returns `record: not_found`.

`yai store record list --case <case_ref> [--limit <N>]` scans
`records_by_case` and resolves matching ids through `records_by_id`.

`yai store record list --kind <record_kind> [--limit <N>]` scans
`records_by_kind` and resolves matching ids through `records_by_id`.

`yai store record list --subject <subject_ref> [--limit <N>]` scans
`records_by_subject` and resolves matching ids through `records_by_id`.

`yai store record list --receipt <receipt_ref> [--limit <N>]` scans
`records_by_receipt` and resolves matching ids through `records_by_id`.

If the LMDB environment is missing, uninitialized or unavailable, get/list
commands print record-store status fields. They do not read from journal as a
fallback and they do not fake records.

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

SPINE.30 implements:

```text
records_by_id
records_by_case
records_by_kind
schema_meta
```

SPINE.31 reads:

```text
records_by_id
records_by_case -> records_by_id
records_by_kind -> records_by_id
```

SPINE.32 adds:

```text
records_by_subject
records_by_receipt
```

Future waves may add:

```text
records_by_projection
records_by_time
record_meta
```

They are rebuildable from journal residue. LMDB must not replace journal replay
or audit.

## SPINE.34 Freeze

SPINE.34 LMDB Record Plane Freeze accepts the current `yai.record.v1` surface
as stable enough for journal replay and graph persistence work to build on.

Frozen:

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
schema_meta
```

There is no journal fallback. Missing, not initialized or unavailable LMDB
environments report record-store status fields instead of synthesizing records
from journal residue.

The freeze also validates that the record plane can preserve carrier/control
record kinds:

```text
carrier_request
effect_receipt
filesystem_receipt
divergence
reconciliation
```

This is storage/index validation only. It does not add new carrier execution,
journal replay, graph persistence, facts, memory or projection behavior.

## SPINE.35 Journal Inspect Boundary

SPINE.35 adds:

```text
yai journal inspect --path <journal.jsonl> [--show-errors]
```

Journal is replay/audit. LMDB is durable indexed record lookup. The inspect
command reports invalid_json, invalid_schema, unsupported_kind, duplicate and
replay_ready status. It does not write LMDB, rebuild LMDB or create a no silent
journal fallback path for record reads.

## SPINE.36 Journal Replay Boundary

SPINE.36 adds:

```text
yai journal replay --path <journal.jsonl> [--dry-run]
```

Replay writes valid journal records through the existing LMDB record writer.
It populates records_by_id, records_by_case, records_by_kind,
records_by_subject and records_by_receipt. Existing records are reported as
records_duplicate and are not written again.

Dry-run writes nothing. Invalid input blocks replay with records_written: 0.
Store reads still have no journal fallback.

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

SPINE.30 also read-only inspected the same store/record/query/execution receipt
areas for write-path posture. The old material stays concept evidence; no
`yai-dev` file is mutated.

SPINE.31 read-only inspected query, index, store and records material for
record query grammar, kind filters, case filters, record listing and index/read
path posture. The old material stays concept evidence; no `yai-dev` file is
mutated.

SPINE.32 read-only inspected query, index, store, records and operational
receipt material for subject indexes, receipt lookup, record relationship
indexes, query filters and index consistency posture. The old material stays
concept evidence; no `yai-dev` file is mutated.

SPINE.33 read-only inspected query, index and store material for query surface
wording, no-fallback reads, zero-result posture and manual lookup validation.
The old material stays concept evidence; no `yai-dev` file is mutated.

SPINE.34 read-only classified the same store/index/query/receipt material for
record-plane freeze posture. No additional `yai-dev` source is migrated or
mutated.

## Write Path

The Rust engine owns the LMDB backend via the `lmdb` crate. The C daemon still
writes journal residue first. The `yai` CLI mirrors completed daemon-loop
journals into LMDB by loading the emitted JSONL and writing:

```text
record:id:<record_id> -> yai.record.v1 envelope
record:case:<case_ref>:<record_id> -> record_id
record:kind:<record_kind>:<record_id> -> record_id
record:subject:<subject_ref>:<record_id> -> record_id
record:receipt:<receipt_ref>:<record_id> -> record_id
meta:schema -> yai.record.v1
meta:rebuild -> not_started
```

If LMDB import fails, the command fails explicitly and names that the journal
path remains written. The failure must not be silent.

## Validation

SPINE.29-SPINE.30 guards:

```text
make check-lmdb-record-plane-doctrine
make smoke-spine29
make smoke-spine30
make smoke-spine31
make smoke-spine32
make smoke-spine33
make smoke-spine34
```

They are integrated into:

```text
make check-docs
make smoke
make check
```
