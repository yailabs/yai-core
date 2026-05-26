# Journal Replay Freeze

Status: SPINE.39 Journal Replay Freeze.

## Purpose

SPINE.39 freezes the journal replay block before graph persistence and
RuntimeGraph work begins.

The frozen replay chain is:

```text
journal.jsonl
-> inspect
-> replay --dry-run
-> replay to LMDB
-> replay-status
-> replay-report
-> store summary / query
```

## Frozen Boundary

Journal remains append-only replay/audit residue. LMDB remains durable indexed
record lookup. Replay is the controlled materialization path from journal to
LMDB.

Replay must never:

```text
silently skip invalid input
silently fall back from LMDB query to journal reads
report false completion
materialize incompatible schemas
hide corrupt input
```

## Frozen Command Surface

SPINE.39 adds no command.

The frozen replay commands are:

```bash
yai journal inspect --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
yai journal replay --path <journal.jsonl>
yai journal replay-status --path <journal.jsonl>
yai journal replay-report --path <journal.jsonl>
yai store summary
yai store record list --case <case_ref> --limit <N>
```

## Required Fields

Replay diagnostics must expose:

```text
journal_identity
record_schema
journal_schema
compatibility
cursor_line
replay_status
records_written
records_duplicate
records_skipped
invalid_entries
replay_report_schema: yai.replay_report.v1
```

## Canonical Lab

Filesystem loop, journal, LMDB and carrier manual checks live under:

```text
docs/labs/filesystem-loop/
```

The old filesystem-loop manual path is not the active
lab path.

## Freeze Validation

The freeze validates:

```text
valid replay completes
dry-run writes no LMDB records
second replay is idempotent
schema_mismatch blocks writes
invalid_json blocks writes
completed replay produces completed report
failed replay produces failed report
LMDB query has no journal fallback
```

## Non-Goals

```text
No new replay feature.
No graph rebuild.
No DuckDB facts.
No memory consolidation.
No RuntimeGraph.
No HNSW.
No carrier/provider changes.
```
