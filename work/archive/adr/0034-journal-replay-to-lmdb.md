# ADR 0034: Journal Replay to LMDB

## Status

Accepted for SPINE.36.

## Context

SPINE.34 froze the LMDB record plane. SPINE.35 added diagnostic journal inspect
and replay readiness reporting.

The next step is materialization:

```text
journal.jsonl -> yai.store.record.v0 -> yai.record.v1 -> LMDB indexes
```

## Decision

Journal replay to LMDB is implemented as a diagnostic, idempotent replay path.
This ADR uses journal replay to LMDB as the canonical materialization phrase.

Replay uses the existing LMDB record writer and index path. It writes:

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
```

Replay refuses non-ready journals. Invalid input is reported through
`invalid_entries`, `unsupported_entries`, `duplicate_entries` and `reason`.
There is no silent skip and no best-effort materialization in SPINE.36.

The replay command is:

```bash
yai journal replay --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
```

Dry-run does not write LMDB. Real replay writes only records not already present
in `records_by_id`. Existing records are counted as `records_duplicate`, making
second replay idempotent. New records are counted as `records_written`.

## Consequences

LMDB can now be rebuilt from a valid journal manually. This does not change the
SPINE.34 no journal fallback rule: store queries still read LMDB only.

SPINE.37 must harden schema version handling, replay cursor metadata and replay
compatibility semantics.

## Non-Goals

```text
No graph rebuild.
No DuckDB facts.
No memory consolidation.
No RuntimeGraph implementation.
No HNSW.
No provider/runtime work.
No carrier behavior changes.
```
