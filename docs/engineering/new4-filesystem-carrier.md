# NEW.4 Filesystem Carrier

NEW.4 introduces the first real controlled effect path. It is intentionally
limited to a test sandbox and does not add process, network, model, database or
daemon IPC behavior.

## Implemented Surface

```text
filesystem carrier v0
filesystem subject locator v0
fs.read
fs.write
before/after hash posture
filesystem receipt fields
journal persistence
subject state update
filesystem projection counts
yai receipt inspection
```

## Sandbox Rule

Filesystem effects may only target paths under:

```text
build/tmp/new4/sandbox/
```

The carrier rejects paths outside the configured sandbox and rejects `..`
segments. A blocked or failed receipt is emitted instead of writing.

## Operations

`fs.read`:

```text
path must be inside sandbox
file is read for bytes/hash posture
receipt status = observed
```

`fs.write`:

```text
path must be inside sandbox
decision must be allow or allow_with_constraints
before_hash is recorded when a file exists
after_hash is recorded after write
receipt status = executed
```

If the decision is `require_review` or `deny`, the carrier does not write and
emits a blocked receipt.

## Receipt Fields

`yai_effect_receipt_t` now carries:

```text
carrier_kind
effect_kind
path_or_locator
bytes_affected
before_hash
after_hash
status
```

NEW.4 uses deterministic FNV-1a posture hashes. They are sufficient for this
smoke path and are not a cryptographic commitment.

## Projection

Projection counts filesystem residue:

```text
filesystem_receipts
subject_states
effects
```

## Debug Commands

```text
yai receipt summary --journal build/tmp/new4/journal.jsonl
yai carrier fs-read --sandbox build/tmp/new4/sandbox --path build/tmp/new4/sandbox/input.txt
yai carrier fs-write --sandbox build/tmp/new4/sandbox --path build/tmp/new4/sandbox/manual.txt --content "hello"
```

## Out Of Scope

```text
no process carrier
no network carrier
no model carrier
no database carrier
no repository/git carrier
no daemon IPC
no full sandbox engine
no permission model beyond sandbox path confinement
no concurrency or locking
```

## Validation

```text
make smoke-new4
make smoke
make check
cargo fmt --check
git diff --check
```
