# ADR 0036: Journal Replay Freeze

## Status

Accepted for SPINE.39.

## Context

SPINE.35 through SPINE.38 built the replay block:

```text
journal inspect
journal replay to LMDB
replay idempotency / schema / cursor / status
durable replay report
```

The next work block is graph persistence and RuntimeGraph. Replay must be
stable before durable graph truth starts depending on it.

## Decision

The journal replay block is frozen as the controlled path for materializing
append-only replay/audit residue into LMDB with diagnostics, idempotency,
schema compatibility, cursor metadata and durable replay reports.

The frozen command surface is:

```bash
yai journal inspect --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
yai journal replay --path <journal.jsonl>
yai journal replay-status --path <journal.jsonl>
yai journal replay-report --path <journal.jsonl>
```

Replay reports use:

```text
replay_report_schema: yai.replay_report.v1
journal_identity
compatibility
cursor_line
schema_mismatch
invalid_json
idempotent
```

## Consequences

Graph persistence and RuntimeGraph work can depend on a replay block that is
inspectable, idempotent, schema-aware and report-producing.

Invalid input, schema mismatch and corrupt JSON remain diagnostic failures.
They must not become silent skips or false completed rebuilds.

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
