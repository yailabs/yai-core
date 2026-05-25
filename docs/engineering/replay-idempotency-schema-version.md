# Replay Idempotency Schema Version

Status: SPINE.37 Replay Idempotency + Schema Version Handling.

## Executive Summary

SPINE.37 hardens journal replay with replay cursor metadata, journal identity
and schema compatibility posture.

Replay is no longer only "write missing records and count duplicates". It now
records the replay status needed for future rebuild diagnostics:

```text
journal_identity
record_schema
journal_schema
compatibility
cursor_line
replay_status
records_written
records_duplicate
```

## Command Surface

```bash
yai journal replay --path <journal.jsonl> --dry-run
yai journal replay --path <journal.jsonl>
yai journal replay-status --path <journal.jsonl>
```

SPINE.38 adds the durable report companion:

```bash
yai journal replay-report --path <journal.jsonl>
```

The report schema is `yai.replay_report.v1` and carries `journal_identity`,
`compatibility`, `cursor_line`, `records_written`, `records_duplicate`,
`invalid_entries` and failed report diagnostics for audit after replay.

No commands are removed or deprecated.

## Schema Handling

SPINE.37 reports two schema concepts:

```text
journal_schema = yai.store.record.v0
record_schema  = yai.record.v1
```

`yai.store.record.v0` is the accepted journal input schema for current replay.
`yai.record.v1` is the LMDB record plane schema produced by replay.

Schema mismatch behavior:

```text
compatibility: schema_mismatch
replay_ready: no
records_written: 0
replay_status: incompatible
```

Missing schema input is not accepted silently. Current behavior reports
`invalid_schema`. If a future legacy converter accepts missing schema input, it
must report `compat_legacy` explicitly.

## Replay Cursor

The cursor is line-based in SPINE.37:

```text
cursor_line: N
```

After a completed replay, `cursor_line` equals the total journal line count.
Dry-run reports the current cursor and `would_update_cursor: yes` without
writing metadata.

## Replay Metadata

Replay metadata is stored under the LMDB metadata namespace:

```text
meta:replay:<journal_identity>
```

Fields:

```text
replay_id
journal_identity
journal_path
record_schema
journal_schema
started_at
completed_at
lines_total
lines_replayed
records_written
records_duplicate
records_skipped
invalid_entries
unsupported_entries
cursor_line
status
compatibility
```

## Required Outputs

Dry-run:

```text
journal_replay: dry_run
journal_identity: ...
record_schema: yai.record.v1
journal_schema: yai.store.record.v0
compatibility: compatible
cursor_line: 0
would_update_cursor: yes
replay_ready: yes
```

Replay status:

```text
journal_replay_status:
journal_identity: ...
record_schema: yai.record.v1
cursor_line: N
replay_status: completed
compatibility: compatible
```

Schema mismatch:

```text
journal_replay: failed
compatibility: schema_mismatch
supported_schema: yai.record.v1
records_written: 0
replay_status: incompatible
```

## Validation

```bash
make check-replay-idempotency-schema-version
make smoke-spine37
```

Smoke verifies:

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
No old docs/manuals path updates.
```

## Freeze Note

SPINE.39 freezes replay cursor and compatibility output. `replay-status` must
continue to expose `journal_identity`, `record_schema`, `journal_schema`,
`compatibility`, `cursor_line`, `replay_status` and idempotent replay posture,
and schema_mismatch must not be reported as completed replay.
