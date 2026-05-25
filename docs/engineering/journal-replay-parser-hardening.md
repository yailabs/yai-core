# Journal Replay Parser Hardening

Status: SPINE.35 Journal Replay Doctrine + Parser Hardening.

## Executive Summary

SPINE.35 adds the first journal replay boundary surface. Journal is replay/audit
chronology. LMDB is durable indexed record lookup. The new inspection command
checks whether a journal is replay-ready without writing LMDB or performing a
rebuild.

The command is:

```bash
yai journal inspect --path <journal.jsonl> [--show-errors]
```

## Boundary

Journal inspect is parser hardening, not replay execution.

It may:

```text
read a JSONL journal
count valid entries
report invalid_json
report invalid_schema
report unsupported_kind
report duplicate
emit replay diagnostics
report replay_ready yes|no
```

It must not:

```text
write LMDB
rebuild LMDB
repair a journal
silently skip corrupt lines
create a no silent journal fallback exception
```

## Parser Policy

The default parser policy is:

```text
diagnostic
```

Diagnostic mode reports errors and returns a readable summary. It does not
pretend full success when any invalid, unsupported or duplicate line exists.

## Status Taxonomy

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

`replayed` is reserved for later journal-to-LMDB work. SPINE.35 only inspects.

## Required Output

Summary output:

```text
journal_path: ...
parser_policy: diagnostic
lmdb_write: no
journal_status: readable|missing|unavailable
lines_total: N
valid_entries: N
invalid_entries: N
unsupported_entries: N
duplicate_entries: N
replay_ready: yes|no
```

With `--show-errors`, each replay diagnostic prints:

```text
line: N
entry_status: invalid_json|invalid_schema|unsupported_kind|duplicate
record_id: ...
record_kind: ...
schema: ...
error_code: ...
error_message: ...
action: report
```

## Idempotency

SPINE.35 does not implement replay idempotency. It prepares for idempotency by
making duplicate record ids visible during inspection. SPINE.36 and SPINE.37
must preserve this rule when materializing records into LMDB.

## Validation

```bash
make check-journal-replay-boundary
make smoke-spine35
```

The smoke verifies valid, missing, corrupt, duplicate and empty journal
inspection. It also checks that inspection does not create an LMDB path.

## Next

SPINE.36 Journal Replay to LMDB may materialize replay into the frozen
`yai.record.v1` plane. It must keep replay diagnostics visible and preserve the
SPINE.34 LMDB no silent journal fallback rule.
