# Journal Replay to LMDB

Status: SPINE.36 Journal Replay to LMDB.

## Purpose

SPINE.36 implements the first explicit journal replay to LMDB path.

Journal remains replay/audit chronology. LMDB remains durable indexed record
lookup. Replay materializes valid append-only residue into the frozen
`yai.record.v1` record plane through the existing LMDB writer.

## Boundary

The replay pipeline is:

```text
journal.jsonl
-> diagnostic parser
-> yai.store.record.v0 records
-> yai.record.v1 envelopes
-> records_by_id
-> records_by_case
-> records_by_kind
-> records_by_subject
-> records_by_receipt
```

Replay is not graph rebuild, DuckDB fact derivation, memory consolidation,
RuntimeGraph loading or HNSW indexing.

## Command

```bash
yai journal replay --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
```

Dry-run reports what would be written and does not open or create the LMDB
environment.

## Replay Rules

Replay must be diagnostic:

```text
invalid_entries must block real replay
unsupported_entries must block real replay
duplicate journal record ids must block real replay
no silent skip
```

Replay must be idempotent:

```text
existing records are counted as records_duplicate
existing records are not written again
second replay of the same journal reports records_written: 0
```

Replay must preserve the SPINE.34 record-plane rule:

```text
no journal fallback
```

LMDB query commands still read LMDB only. Replay creates LMDB material; it does
not make store queries synthesize records from journal.

## Report Shape

```text
journal_path
lmdb_path
lines_total
valid_entries
invalid_entries
unsupported_entries
duplicate_entries
records_seen
records_written
records_duplicate
records_skipped
replay_ready
idempotent
status
```

## Roadmap Placement

SPINE.36 provides basic replay and idempotent replay behavior. SPINE.37 will
harden schema version handling, replay cursor and compatibility posture.
