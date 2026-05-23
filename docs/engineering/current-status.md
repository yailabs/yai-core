# Current Engineering Status

Status: SPINE.20 local runtime layout.

## Completed Foundation

NEW.0 through NEW.19 are complete foundation history. SPINE.6A compressed
active engineering docs. SPINE.6B adds the operational extraction contract for
future implementation waves. SPINE.20 adds the local runtime layout.

The next active roadmap item is:

```text
SPINE.21 Filesystem Refactor Freeze
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
testing.md
wave-template.md
operational-extraction-contract.md
```

## Known Caveats

The current data plane remains partly journal-backed and partly transitional C
smoke support. Shared memory, LMDB, Ladybug, DuckDB, projection deltas, memory
consolidation and cross-plane reconcile remain future SPINE.22-SPINE.30 work.
SPINE.20 creates `YAI_HOME/store` as the future durable data-plane root but does
not create those backends.

Old `yai` residue for SPINE.20 was read-only inspected. The old `yai` worktree
has an existing dirty architecture file, so no old-yai files were modified.
The extraction inventory records that caveat as `blocked_by_dirty_worktree`.

Future implementation waves must classify corresponding old-yai residue. A
wave is not complete until old material has been absorbed, rewritten, split,
externalized, quarantined, marked compatibility-only, marked delete-later, left
untouched with a reason, or explicitly deferred.

Archived historical records live under:

```text
docs/archive/engineering/
```
