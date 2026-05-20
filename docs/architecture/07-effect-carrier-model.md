# Effect And Carrier Model

An effect is the operational consequence attempted, executed, observed or imported.

A carrier is the controlled mechanism that can execute, observe or import evidence for an effect.

## Carrier Responsibilities

```text
carrier registry
carrier request
execution or observation
result normalization
receipt production
external receipt import
failure classification
```

## Initial Carriers

```text
null
filesystem
process
repository
model
network
database
event
external_receipt
```

## Receipt Rule

No effect path is complete until it has a receipt, a missing-receipt divergence, or an explicit observe-only record.

## Boundary

Carriers do not own policy. Control decides. Carriers execute or observe. Store persists receipts. Graph reconstructs relationships.

## NEW.4 Filesystem Carrier V0

NEW.4 starts carrier-owned effects with a sandbox-only filesystem carrier.

Supported operations:

```text
fs.read
fs.write
```

Safety rule:

```text
all filesystem writes must remain inside build/tmp/new4/sandbox/
paths with parent traversal are rejected
```

`fs.read` reads an existing file inside the sandbox and emits an observed
receipt with bytes and content hash. `fs.write` writes only when the control
decision is `allow` or `allow_with_constraints`; `require_review` and `deny`
produce blocked receipts and do not mutate the file.

Receipt posture:

```text
carrier_kind = filesystem
effect_kind = fs.read or fs.write
path_or_locator
bytes_affected
before_hash
after_hash
status
```

NEW.4 does not implement a general sandbox engine, filesystem event stream,
process execution, network transport or daemon IPC.
