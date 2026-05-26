Historical/superseded engineering record. Not an active source of truth.

# SPINE.25 Hot State Case Session / Context Integration

Historical engineering record. Not an active source of truth.

## Outcome

SPINE.25 connects the hot-state snapshot to live case lifecycle:

```text
case_session_status
case_world_status
case_context_status
active_thread_id
participant_view_frame_id
decision/receipt projection staleness
projection refresh freshness
```

Hot state remains a non-authoritative runtime cache. Durable truth remains in
residue: journal/records today, later LMDB, graph, facts, memory and reconcile.

## Command Surface

Added commands:

```text
none
```

Changed commands:

```text
yai hot status
yai doctor
```

Removed/deprecated commands:

```text
none
```

Manual command tests:

```text
target/debug/yai doctor
target/debug/yai hot status
yai daemon run-minimum-loop --socket <socket>
yai daemon run-filesystem-loop --socket <socket>
yai hot status
```

Expected key output:

```text
case_session: active
case_world: loaded
case_context: active
projection: fresh|stale|unknown|rebuilding
stale_reason: ...
```

## Validation Target

```text
make smoke-spine25
make check
```
