# Replay Idempotency Schema Version

Status: SPINE.37 Replay Idempotency + Schema Version Handling.

## Purpose

SPINE.37 makes journal replay formally idempotent and schema-version aware.

SPINE.36 proved that a valid `journal.jsonl` can materialize records into the
LMDB record plane. SPINE.37 adds the metadata needed to know what was replayed,
from which journal identity, under which schema posture and up to which cursor.

## Boundary

Replay remains a materialization boundary:

```text
journal.jsonl
-> diagnostic parser
-> schema compatibility check
-> replay metadata
-> yai.record.v1 LMDB envelopes
-> replay cursor
```

Journal remains replay/audit chronology. LMDB remains durable indexed record
lookup. Store queries must not silently fall back to the journal.

SPINE.38 extends this boundary with `yai.replay_report.v1` durable replay
diagnostics. The report preserves `journal_identity`, `compatibility`,
`cursor_line`, `records_written`, `records_duplicate`, `invalid_entries` and
failed report evidence after the replay command exits.

## Schema Posture

YAI separates the input journal schema from the LMDB record schema:

```text
journal_schema = yai.store.record.v0
record_schema  = yai.record.v1
```

The replay command reports both. A replay may be:

```text
compatible
schema_mismatch
unsupported_schema
unknown
```

Missing schema input is not treated as modern. Current behavior reports it as
`invalid_schema`; future legacy compatibility must be explicit as
`compat_legacy`.

## Replay Cursor

The SPINE.37 cursor is line-based:

```text
journal_identity
journal_path
cursor_line
last_record_schema
status
compatibility
```

Byte offset can be added later. Line number is enough for this v0 metadata
surface.

## Replay Metadata

Replay metadata is stored in the LMDB metadata namespace:

```text
meta:replay:<journal_identity>
```

The metadata shape includes:

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

## Status Rules

Replay statuses:

```text
not_started
in_progress
completed
partial
failed
incompatible
```

SPINE.37 does not introduce best-effort replay. Invalid, corrupt or incompatible
input must not produce a false `completed` status.

## Command Surface

```bash
yai journal replay --path <journal.jsonl> --dry-run
yai journal replay --path <journal.jsonl>
yai journal replay-status --path <journal.jsonl>
```

Dry-run reports the planned cursor update but does not write LMDB. Real replay
writes records and persists replay metadata. Replay status reads metadata by
journal identity.

## Non-Goals

```text
No graph rebuild.
No DuckDB facts.
No memory consolidation.
No RuntimeGraph.
No HNSW.
No carrier/provider changes.
```
