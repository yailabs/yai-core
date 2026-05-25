# Journal Replay to LMDB

Status: SPINE.36 Journal Replay to LMDB.

## Executive Summary

SPINE.36 adds real journal replay to LMDB.

Journal remains replay/audit. LMDB remains durable indexed record lookup.
Replay reads a valid `journal.jsonl`, converts recognized `yai.store.record.v0`
records into `yai.record.v1` envelopes and writes them through the frozen LMDB
record/index path.

## Command Surface

```bash
yai journal replay --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
```

Use inspect before replay:

```bash
yai journal inspect --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
yai journal replay --path <journal.jsonl>
yai store summary
yai store record list --case <case_ref> --limit 10
```

## Dry-Run

Dry-run does not write LMDB:

```text
journal_replay: dry_run
journal_path: ...
lmdb_path: <YAI_HOME>/store/lmdb
lines_total: N
valid_entries: N
invalid_entries: 0
records_to_write: N
would_write_lmdb: yes
replay_ready: yes
```

## Real Replay

Real replay writes valid records into:

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
```

Report fields include:

```text
journal_identity
record_schema
journal_schema
compatibility
records_seen
records_written
records_duplicate
records_skipped
cursor_line
replay_status
record_store_status
idempotent
```

## Idempotent Replay

Idempotent replay is basic in SPINE.36:

```text
records already present in LMDB are records_duplicate
records_duplicate are not written again
records_written: 0 on second replay of the same journal
idempotent: yes when all seen records already existed
```

SPINE.37 hardens idempotency with schema version, replay cursor, metadata and
compatibility handling. Use:

```bash
yai journal replay-status --path <journal.jsonl>
```

## Failure Posture

Replay fails before writing when the journal is not replay-ready:

```text
invalid_entries: N
unsupported_entries: N
duplicate_entries: N
records_written: 0
replay_ready: no
reason: invalid_json|invalid_schema|unsupported_kind|duplicate
```

There is no best-effort mode in SPINE.36 and no silent skip.

## LMDB Boundary

Replay materializes records into LMDB. It does not create a no journal fallback
exception. Store commands continue to read LMDB status and LMDB indexes only.

## Validation

```bash
make check-journal-replay-to-lmdb
make smoke-spine36
```

Smoke verifies dry-run, real replay, idempotent replay, corrupt journal failure
and LMDB store summary/query behavior.
