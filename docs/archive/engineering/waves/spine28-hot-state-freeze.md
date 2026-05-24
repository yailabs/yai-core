Historical/superseded engineering record. Not an active source of truth.

# SPINE.28 Hot State Freeze

Status: complete.

Purpose: freeze the hot-state block before SPINE.29 starts LMDB durable record
plane work.

Frozen boundary:

```text
hot state is not truth
journal remains replay/audit
YAI_HOME/run/hot-state.json remains schema yai.hot_state.v1
missing_snapshot and invalid_snapshot are explicit non-crashing status cases
case_session/case_world/case_context fields are live diagnostic cache
projection_freshness/stale_reason/freshness_policy remain command-visible
LMDB will not replace hot state
```

Command surface:

```text
Added commands: none
Changed commands: none
Removed/deprecated commands: none

Stable commands:
  yai doctor
  yai hot status
  yai projection inspect --journal <journal> --consumer <consumer>
  make check-hot-state-freeze
  make smoke-spine23
  make smoke-spine24
  make smoke-spine24a
  make smoke-spine25
  make smoke-spine26
  make smoke-spine27
```

Validation target:

```text
make check-hot-state-freeze
make check-docs
make smoke-spine23
make smoke-spine24
make smoke-spine24a
make smoke-spine25
make smoke-spine26
make smoke-spine27
make smoke
make check
```

Next:

```text
SPINE.29 LMDB Record Plane Doctrine + Schema
```
