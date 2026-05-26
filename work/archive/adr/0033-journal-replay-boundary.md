# ADR 0033: Journal Replay Boundary

## Status

Accepted for SPINE.35.

## Context

SPINE.34 froze the LMDB record plane as `yai.record.v1` durable indexed lookup
with id, case, kind, subject and receipt indexes. The next data-plane work must
make journal replay robust before rebuilding LMDB from journal residue.

Journal is replay/audit chronology. LMDB is durable indexed record lookup.
There is no silent journal fallback for LMDB query behavior.

## Decision

YAI hardens the journal replay boundary before full replay execution.

`yai journal inspect --path <journal.jsonl> [--show-errors]` reads a journal,
reports parser status and emits replay diagnostics. It does not write LMDB,
rebuild records or repair corrupt input.

The parser status vocabulary is:

```text
valid
invalid_json
invalid_schema
unsupported_kind
duplicate
replayed
skipped_with_reason
unknown
```

Replay diagnostics are mandatory before replay materialization:

```text
journal_path
line_number
entry_status
record_id
record_kind
schema
error_code
error_message
action
```

Replay idempotency is prepared by exposing duplicate record ids during inspection.
SPINE.36 and SPINE.37 must preserve that visibility when replay writes begin.

## Consequences

Corrupt journal lines become visible as invalid_json or invalid_schema. Unknown
record kinds become unsupported_kind. Duplicate record ids become duplicate.

Partial replay readiness is explicit. The command reports `replay_ready: no`
when inspection finds invalid, unsupported or duplicate entries.

SPINE.35 keeps the no silent journal fallback rule intact. LMDB commands must
continue to report LMDB status rather than fabricating journal-backed lookup
results.
