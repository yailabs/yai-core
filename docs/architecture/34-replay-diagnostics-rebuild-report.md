# Replay Diagnostics / Rebuild Report

SPINE.38 adds durable replay diagnostics for journal to LMDB materialization.

Journal remains the replay/audit chronology. LMDB remains the durable record
plane. Replay is the materialization operation that carries metadata, cursor,
compatibility and now a durable report.

## Report Schema

Replay reports use `yai.replay_report.v1` and are written under:

```text
YAI_HOME/store/replay/reports/<journal_identity>.replay-report.json
```

The report records `journal_identity`, `journal_path`, `lmdb_path`,
`record_schema`, `journal_schema`, `compatibility`, `cursor_line`,
`records_written`, `records_duplicate`, `invalid_entries`, replay readiness,
idempotence, failed report diagnostics and final replay status.

## Operational Rule

A replay status without a report is insufficient for audit, debugging and
future conformance. Completed, idempotent and failed replay attempts must leave
inspectable evidence.

## Boundary

SPINE.38 does not add graph rebuild, DuckDB facts, memory consolidation,
RuntimeGraph, HNSW, carrier/provider behavior or interfaces/studio behavior.
