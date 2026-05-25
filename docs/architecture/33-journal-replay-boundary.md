# Journal Replay Boundary

Status: SPINE.35 Journal Replay Doctrine + Parser Hardening.

## Purpose

SPINE.35 defines the boundary between append-only journal residue and the frozen
LMDB record plane before full journal-to-LMDB replay exists.

Journal is replay/audit chronology. LMDB is durable indexed record lookup. A
journal can rebuild derived or durable planes, but LMDB query behavior must use
LMDB status and lookup semantics directly. There is no silent journal fallback.

## Doctrine

Journal is replay/audit. LMDB is durable indexed record lookup.

Replay must be diagnostic before it becomes destructive or materializing:

```text
journal line
-> parser status
-> replay diagnostics
-> replay_ready yes|no
-> later replay engine
```

A corrupt journal line is not silently skipped. A partial replay must produce
diagnostics. A replayed record must support idempotency. A schema mismatch must
be visible.

## Entry Status

SPINE.35 makes these parser statuses visible:

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

`replayed` is reserved for later replay engines. `valid` means the parser can
recognize the line as a supported `yai.store.record.v0` record. `duplicate`
means the same record id appeared earlier in the inspected journal.

## Replay Diagnostics

Replay diagnostics carry:

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

SPINE.35 exposes this through `yai journal inspect --path <journal.jsonl>` and
`yai journal inspect --path <journal.jsonl> --show-errors`.

## Parser Policy

The initial policy is:

```text
diagnostic
```

Diagnostic mode collects and reports errors. It does not pretend that the
journal is replay-ready when invalid_json, invalid_schema, unsupported_kind or
duplicate lines exist.

## LMDB Boundary

SPINE.35 does not write LMDB. It does not rebuild the record plane. It does not
change the frozen SPINE.34 no silent journal fallback rule for LMDB queries.

`yai journal inspect` reports replay readiness only. It is not a replay command.

## Roadmap Placement

SPINE.35 hardens parser and doctrine. SPINE.36 can then implement journal
replay to LMDB against the frozen `yai.record.v1` lookup and index posture.
