# ADR 0035: Replay Idempotency Schema Version

## Status

Accepted for SPINE.37.

## Context

SPINE.36 added real journal replay to LMDB with base idempotency. The replay cursor
metadata was not yet persisted. That was
enough to materialize records, but not enough to reason about future rebuilds.

Rebuilds need to know:

```text
which journal was replayed
which schema was accepted
which record schema was produced
which cursor was reached
whether replay completed, failed or was incompatible
```

## Decision

Journal replay must persist replay metadata, schema compatibility and replay
cursor state so rebuilds are idempotent and diagnosable.

SPINE.37 stores replay metadata in the LMDB metadata namespace:

```text
meta:replay:<journal_identity>
```

The CLI exposes:

```bash
yai journal replay-status --path <journal.jsonl>
```

`yai journal replay` and `yai journal replay --dry-run` now report:

```text
journal_identity
record_schema
journal_schema
compatibility
cursor_line
replay_status
```

Current replay accepts `yai.store.record.v0` journal entries and materializes
them into `yai.record.v1` LMDB envelopes. Schema mismatch is rejected without
record writes.

## Consequences

Replay remains idempotent. A second replay counts existing records as
`records_duplicate`, does not write duplicates and preserves a completed cursor.

Invalid or incompatible journals do not get a false completed state. They report
`failed` or `incompatible` with `records_written: 0`.

## Non-Goals

```text
No graph rebuild.
No DuckDB facts.
No memory consolidation.
No RuntimeGraph.
No HNSW.
No carrier/provider changes.
No old legacy manuals surface updates.
```
