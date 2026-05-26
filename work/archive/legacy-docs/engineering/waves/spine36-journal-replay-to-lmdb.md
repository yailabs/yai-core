Historical/superseded engineering record. Not an active source of truth.

# SPINE.36 Journal Replay to LMDB

Status: completed

## Summary

SPINE.36 adds explicit journal replay to LMDB. Replay is diagnostic,
idempotent and uses the frozen `yai.record.v1` LMDB record/index path.

## Command Surface

Added:

```bash
yai journal replay --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
```

Changed:

```text
none
```

Removed/deprecated:

```text
none
```

## Validation

```bash
make check-journal-replay-to-lmdb
make smoke-spine36
```

Expected smoke output:

```text
journal_replay:dry_run ok
journal_replay:write ok
journal_replay:idempotent ok
journal_replay:corrupt invalid_json ok
store_summary:ready ok
```

## Non-Goals

```text
No graph rebuild.
No DuckDB facts.
No memory consolidation.
No RuntimeGraph implementation.
No HNSW.
No provider/runtime work.
No carrier behavior changes.
No yai-dev mutation.
```
