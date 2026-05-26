Historical/superseded engineering record. Not an active source of truth.

# SPINE.28B Internal Source Surface Cleanup

Status: done.

## Decision

`system/` is not a second data engine. `engine/` is the Rust data spine target.

`system/` keeps active C system, case, control, daemon, effect, hot-state,
operation and subject implementation code. The remaining C data roots are
transitional shims until their planes move behind the Rust engine.

## Cleanup

Removed local environment roots from the worktree:

```text
venv/
.venv/
```

Archived README-only ingest placeholders:

```text
system/ingest
include/yai/ingest
```

Added `work/spines/source-surface.md` and
`check-source-surface-clean` to keep the active source surface explicit.

## Non-Goals

No source implementation was moved between C and Rust. No LMDB, graph, fact,
projection-delta, memory or model work was implemented.
