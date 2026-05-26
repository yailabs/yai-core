# Replay Diagnostics / Rebuild Report

SPINE.38 makes replay explainable after the fact.

`yai journal replay --path <journal.jsonl>` now writes a durable JSON report
for real replay attempts and prints `replay_report: <path>`. The report can be
read with:

```bash
yai journal replay-report --path <journal.jsonl>
```

The report schema is `yai.replay_report.v1`. It includes `journal_identity`,
`journal_path`, `lmdb_path`, `record_schema`, `journal_schema`,
`compatibility`, `cursor_line`, `records_written`, `records_duplicate`,
`records_skipped`, `invalid_entries`, `unsupported_entries`,
`duplicate_entries`, `replay_status`, `replay_ready`, `idempotent`, `errors`,
`warnings` and `summary`.

Completed replay reports show what was materialized. A second replay of the same
journal updates the report with idempotent behavior: `records_written: 0`,
`records_duplicate: N` and `idempotent: yes`. Corrupt replay writes a failed
report with `invalid_entries` and diagnostic errors; this is the failed report
operators keep for audit.

Reports live under:

```text
YAI_HOME/store/replay/reports/<journal_identity>.replay-report.json
```

No legacy manual path updates are part of SPINE.38.

SPINE.39 freezes `replay-report` as the durable replay evidence command.
Reports remain `yai.replay_report.v1` and must preserve `journal_identity`,
`compatibility`, `cursor_line`, `records_written`, `records_duplicate`,
`invalid_entries`, idempotent second replay posture and failed reports for
invalid_json input.
