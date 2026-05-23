# Current Engineering Status

Status: SPINE.21 pack materialization doctrine.

## Completed Foundation

NEW.0 through NEW.19 are complete foundation history. SPINE.6A compressed
active engineering docs. SPINE.6B adds the operational extraction contract for
future implementation waves. SPINE.20 adds the local runtime layout. SPINE.20A
rebases the active roadmap so pack materialization becomes the next canonical
spine step. SPINE.21 defines packs as first-class case materialization units.

Current:

```text
SPINE.20 Local Runtime Layout completed.
SPINE.20A Pack Roadmap Rebase completed.
SPINE.21 Pack Materialization Doctrine completed.
```

Next:

```text
SPINE.22 Filesystem Refactor Milestone Freeze.
```

Rationale: packs are now canonized before filesystem freeze and data-plane
implementation because pack material becomes case-world material and future
data-plane input.

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
current-status.md
data-plane-roadmap.md
extraction-plan.md
filesystem-target.md
four-repo-roadmap.md
pack-format.md
pack-roadmap.md
testing.md
wave-template.md
operational-extraction-contract.md
```

## Known Caveats

The current data plane remains partly journal-backed and partly transitional C
smoke support. Shared memory, LMDB, Ladybug, DuckDB, projection deltas, memory
consolidation, cross-plane reconcile and observability/evaluation facts remain
future SPINE.23-SPINE.32 work. Pack material is future data-plane input, but
SPINE.21 does not implement pack records or backends. SPINE.20 creates
`YAI_HOME/store` as the future durable data-plane root but does not create
those backends.

Old `yai` residue for SPINE.21 was read-only inspected. The old `yai` worktree
has an existing dirty `README.md`, so no old-yai files were modified. The
extraction inventory records pack/materialization classifications.

Future implementation waves must classify corresponding old-yai residue. A
wave is not complete until old material has been absorbed, rewritten, split,
externalized, quarantined, marked compatibility-only, marked delete-later, left
untouched with a reason, or explicitly deferred.

Archived historical records live under:

```text
docs/archive/engineering/
```
