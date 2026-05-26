Historical/superseded engineering record. Not an active source of truth.

# NEW.2 Persistent Journal

NEW.2 makes the NEW.1 loop persistent and reconstructable without adding real
carriers, daemon IPC or a full policy engine.

## Persisted Surface

```text
case record
subject binding record
attempt record
decision record
receipt record
```

Records are written as JSONL v0 to:

```text
build/tmp/new2/journal.jsonl
```

This path is a smoke-test/dev path, not the final user store layout.

## Record Format V0

Each line is a JSON object with schema `yai.store.record.v0` and known fields:

```text
record_id
case_ref
case_kind
case_status
record_kind
subject_ref
subject_kind
subject_locator
attempt_id
decision_id
receipt_id
summary
```

The C codec writes and reads only this v0 shape. It is not a general JSON
database and does not accept arbitrary external schemas.

## Subject State V0

Subject state is reconstructed from reloaded journal records:

```text
subject_binding -> bound
receipt         -> observed
```

The state stores the last receipt id and decision id seen for the subject.

## Projection

Projection is built from the reloaded journal, not from live process memory.
NEW.2 proves that a process can write records, exit conceptually, reload those
records and produce an operator projection.

## yai

`yai` remains a Rust client and does not own core semantics. NEW.2 adds
file-based debug commands:

```text
yai store tail --journal build/tmp/new2/journal.jsonl
yai projection summary --journal build/tmp/new2/journal.jsonl
```

## Non-Goals

```text
no daemon IPC
no real filesystem/process/model carriers
no full policy engine
no graph/index engine
no memory consolidation
no Rust engine integration into the C production path
no multi-case store
no encryption
no locking/concurrency
```

## Validation

```text
make smoke-new1
make smoke-new2
make smoke
make check
cargo fmt --check
git diff --check
```
