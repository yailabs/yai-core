Historical/superseded engineering record. Not an active source of truth.

# SPINE.38 Replay Diagnostics / Rebuild Report

Status: completed

SPINE.38 adds `yai.replay_report.v1` as durable operational evidence for
journal to LMDB replay. Real replay writes a report, `yai journal replay-report`
reads it, idempotent replay updates it, and corrupt replay writes a failed
report.

The wave keeps journal as replay/audit chronology and LMDB as durable record
plane. It does not add graph rebuild, DuckDB facts, memory consolidation,
RuntimeGraph, HNSW, carrier/provider changes or old legacy manuals surface updates.
