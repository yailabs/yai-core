# Journal Replay Freeze

Status: SPINE.39 Journal Replay Freeze.

## Executive Summary

SPINE.39 freezes journal replay as the controlled materialization path from
append-only replay/audit residue into the LMDB record plane.

The replay block now has:

```text
SPINE.35 journal inspect / parser diagnostics
SPINE.36 replay journal to LMDB
SPINE.37 replay idempotency / schema / cursor / status
SPINE.38 durable replay report
SPINE.39 freeze validation
```

## Command Surface

Added commands:

```text
none
```

Changed commands:

```text
none
```

Frozen commands:

```bash
yai journal inspect --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
yai journal replay --path <journal.jsonl>
yai journal replay-status --path <journal.jsonl>
yai journal replay-report --path <journal.jsonl>
yai store summary
yai store record list --case <case_ref> --limit <N>
```

## Freeze Rules

```text
Journal remains append-only replay/audit residue.
LMDB remains durable indexed record lookup.
Replay is the controlled materialization path from journal to LMDB.
Replay must not silently skip invalid input.
Replay must not create journal fallback reads for LMDB queries.
Replay must not report false completion.
Freeze shorthand: no silent skip, no false completion, no journal fallback.
```

## Required Output Fields

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

Failure outputs remain explicit:

```text
schema_mismatch
invalid_json
idempotent
```

## Canonical Lab Path

Filesystem loop, journal, LMDB and carrier manual checks use:

```text
labs/filesystem-loop/
```

Old filesystem-loop manual material is not the active
lab path.

## Validation

```bash
make check-journal-replay-freeze
make smoke-spine39
```

Smoke verifies:

```text
replay_freeze:inspect ok
replay_freeze:dry_run ok
replay_freeze:replay ok
replay_freeze:status ok
replay_freeze:report ok
replay_freeze:idempotent ok
replay_freeze:schema_mismatch ok
replay_freeze:corrupt_failed_report ok
replay_freeze:no_journal_fallback ok
```

## Non-Goals

```text
No graph rebuild.
No DuckDB facts.
No memory consolidation.
No RuntimeGraph.
No HNSW.
No carrier/provider changes.
No legacy manual active updates.
```
