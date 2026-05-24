# Current Engineering Status

Status: SPINE.27 Hot State CLI + Manual Validation.

## Completed Foundation

NEW.0 through NEW.19 are complete foundation history. SPINE.6A compressed
active engineering docs. SPINE.6B adds the operational extraction contract for
future implementation waves. SPINE.20 adds the local runtime layout. SPINE.20A
rebases the active roadmap so pack materialization becomes the next canonical
spine step. SPINE.21 defines packs as first-class case materialization units.
SPINE.22 freezes filesystem layout, runtime layout, compact engineering docs,
pack doctrine guards and the operational extraction contract before data-plane
implementation begins. SPINE.23 adds the first live hot-state plane. SPINE.23A
expands the linear roadmap and adds the mandatory command surface contract for
future deliveries. SPINE.24 hardens the runtime hot-state snapshot lifecycle.
SPINE.24A maps SPINE.20-SPINE.24 primitives to command views, manual tests and
expected output.
SPINE.25 links that snapshot to case session, case world and case context
lifecycle. SPINE.26 defines the projection freshness policy used by model,
operator, audit and debug consumers. SPINE.27 stabilizes the manual CLI surface
for hot state, doctor diagnostics and projection freshness inspection.

Current:

```text
SPINE.20 Local Runtime Layout completed.
SPINE.20A Pack Roadmap Rebase completed.
SPINE.21 Pack Materialization Doctrine completed.
SPINE.22 Filesystem & Runtime Layout Freeze completed.
SPINE.23 Hot State Doctrine + ABI completed.
SPINE.23A Roadmap Expansion + Command-Test Contract completed.
SPINE.24 Hot State Runtime Snapshot completed.
SPINE.24A Retroactive Command Surface Recovery completed.
SPINE.25 Hot State Case Session / Context Integration completed.
SPINE.26 Hot State Projection Freshness Integration completed.
SPINE.27 Hot State CLI + Manual Validation current.
```

Next:

```text
SPINE.28 Hot State Freeze.
```

Foundation status:

```text
filesystem refactor closed
runtime layout exists
active docs compact
pack doctrine guarded
operational extraction contract active
hot state v0 active
expanded linear roadmap active
command surface contract active
command surface inventory active
hot-state snapshot v1 active
hot-state snapshot missing/corrupt handling active
hot-state session/context lifecycle fields active
projection freshness policy active
consumer-aware model/operator freshness posture active
hot-state CLI manual validation active
```

## Current Layout

```text
include/
system/
engine/
cmd/
proto/
tests/
docs/
tools/
packaging/
examples/
vendor/
```

## Local Runtime Layout

```text
PREFIX=$HOME/.local
YAI_HOME=$HOME/.yai
$(PREFIX)/bin/yai
$(PREFIX)/bin/yaid
$(YAI_HOME)/run
$(YAI_HOME)/store
$(YAI_HOME)/log
$(YAI_HOME)/tmp
$(YAI_HOME)/cases
$(YAI_HOME)/sockets
$(YAI_HOME)/config
$(YAI_HOME)/run/yaid.sock
$(YAI_HOME)/run/hot-state.json
```

`build/tmp` remains test/lab space. `YAI_HOME` is the local runtime home.

Removed active implementation roots:

```text
lib/
crates/
daemon/
ctl/
```

## Active Engineering Docs

```text
README.md
command-surface.md
current-status.md
data-plane-roadmap.md
extraction-plan.md
filesystem-target.md
four-repo-roadmap.md
hot-state-plane.md
pack-format.md
pack-roadmap.md
testing.md
wave-template.md
operational-extraction-contract.md
```

## Known Caveats

The current data plane remains partly journal-backed and partly transitional C
smoke support. SPINE.23 implements hot-state semantics and a daemon-owned
`YAI_HOME/run/hot-state.json` snapshot, not durable truth. SPINE.24 makes the
snapshot schema `yai.hot_state.v1`, writes through temp-and-rename, and makes
`yai hot status` handle missing and corrupt snapshots. SPINE.25 makes the
snapshot carry `case_session_status`, `case_world_status`,
`case_context_status`, `active_thread_id` and `participant_view_frame_id`.
SPINE.26 adds consumer-aware freshness policy so model/agent views are stricter
than operator, audit and debug views. SPINE.27 makes `yai hot status`,
`yai doctor` and `yai projection inspect` the stable manual inspection surface.
True OS shared memory/mmap, LMDB, Ladybug, DuckDB, projection deltas, memory
consolidation, cross-plane reconcile and observability/evaluation facts remain
future SPINE.28-SPINE.80 work. Pack
material is future data-plane input, but SPINE.21 does not implement pack
records or backends. SPINE.20 creates `YAI_HOME/store` as the future durable
data-plane root but does not create those backends.

Old `yai` residue for SPINE.21 was read-only inspected. The old `yai` worktree
has an existing dirty `README.md`, so no old-yai files were modified. The
extraction inventory records pack/materialization classifications.

Old-yai residue for SPINE.23 was read-only inspected and classified in the
extraction inventory. No old-yai source file was modified.

Old-yai residue for SPINE.24 was read-only inspected and classified in the
extraction inventory. No old-yai source file was modified.

Old-yai command/runtime residue for SPINE.24A was read-only inspected and
classified in the extraction inventory. No old-yai source file was modified.

Old-yai residue for SPINE.25 was read-only inspected and classified in the
extraction inventory. No old-yai source file was modified.

Old-yai residue for SPINE.26 was read-only inspected and classified in the
extraction inventory. No old-yai source file was modified.

Old-yai command/runtime residue for SPINE.27 was read-only inspected and
classified in the extraction inventory. No old-yai source file was modified.

Future implementation waves must classify corresponding old-yai residue. A
wave is not complete until old material has been absorbed, rewritten, split,
externalized, quarantined, marked compatibility-only, marked delete-later, left
untouched with a reason, or explicitly deferred.

Archived historical records live under:

```text
docs/archive/engineering/
```
