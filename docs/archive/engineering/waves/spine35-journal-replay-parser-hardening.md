Historical/superseded engineering record. Not an active source of truth.

# SPINE.35 Journal Replay Doctrine + Parser Hardening

Status: completed

## Summary

SPINE.35 defines journal as replay/audit chronology and LMDB as durable indexed
record lookup. It adds a journal inspection command that reports replay
readiness without writing LMDB.

## Command Surface

Added:

```bash
yai journal inspect --path <journal.jsonl>
yai journal inspect --path <journal.jsonl> --show-errors
```

Changed:

```text
none
```

Removed/deprecated:

```text
none
```

## Parser Status

```text
valid
invalid_json
invalid_schema
unsupported_kind
duplicate
replayed
skipped_with_reason
unknown
```

## Validation

```bash
make check-journal-replay-boundary
make smoke-spine35
```

Expected smoke output:

```text
journal_inspect:valid ok
journal_inspect:missing ok
journal_inspect:corrupt invalid_json ok
journal_inspect:empty ok
journal_inspect:no_lmdb_write confirmed
```

## Non-Goals

```text
No full journal replay to LMDB.
No rebuild command.
No graph rebuild.
No DuckDB/facts.
No memory consolidation.
No carrier changes.
No provider/runtime changes.
No yai-dev mutation.
```
