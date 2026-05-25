# ADR 0034: Replay Diagnostics / Rebuild Report

## Status

Accepted.

## Decision

Replay/rebuild operations must produce durable diagnostic reports. A replay
status without a report is insufficient for audit, debugging and future
conformance.

Reports use `yai.replay_report.v1` and expose `journal_identity`,
`compatibility`, `cursor_line`, `records_written`, `records_duplicate`,
`invalid_entries` and failed report diagnostics.

Operators inspect reports with `yai journal replay-report --path
<journal.jsonl>`.

## Consequences

Operators can inspect completed, idempotent and failed replay attempts after the
CLI process exits. Future conformance can use the report as operational
evidence without treating it as a fixture yet.
