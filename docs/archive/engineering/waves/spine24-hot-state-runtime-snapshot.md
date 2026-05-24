Historical engineering record. Not an active source of truth.

# SPINE.24 Hot State Runtime Snapshot

Purpose: harden the hot-state runtime snapshot as a versioned, atomically
written and manually inspectable liveness surface.

Command surface:

```text
Added commands:
  none

Changed commands:
  yai hot status
  yai doctor

Removed/deprecated commands:
  none

Manual command tests:
  target/debug/yai doctor
  target/debug/yai hot status
  target/debug/yai daemon run-minimum-loop --socket <socket>
  target/debug/yai hot status
```

Expected outputs:

```text
hot_state: unavailable
reason: missing_snapshot
reason: invalid_snapshot
hot_state: active
schema: yai.hot_state.v1
case_session: active
case_context: active
hot_state_schema_status: ok
hot_state_readable: yes
```

Non-goals:

```text
no LMDB
no Ladybug
no DuckDB
no projection delta engine
no true mmap/shared memory optimization
```
