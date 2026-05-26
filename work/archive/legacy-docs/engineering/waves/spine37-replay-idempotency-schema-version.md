Historical/superseded engineering record. Not an active source of truth.

# SPINE.37 Replay Idempotency + Schema Version Handling

Status: completed

## Summary

SPINE.37 hardens journal replay with schema compatibility, replay cursor and
metadata status.

The replay path remains:

```text
journal.jsonl -> diagnostic parser -> LMDB record plane
```

but now records:

```text
journal_identity
record_schema
journal_schema
compatibility
cursor_line
replay_status
```

## Command Surface

Added:

```bash
yai journal replay-status --path <journal.jsonl>
```

Changed:

```bash
yai journal replay --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
```

They now report replay metadata and compatibility posture.

## Validation

```bash
make check-replay-idempotency-schema-version
make smoke-spine37
```

Smoke output:

```text
replay:idempotent ok
replay:cursor ok
replay:schema_mismatch ok
replay:status completed
replay:no_false_complete ok
```

## Non-Goals

```text
No graph rebuild.
No DuckDB facts.
No memory consolidation.
No RuntimeGraph.
No HNSW.
No carrier/provider changes.
```
