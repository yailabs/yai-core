# Current Engineering Status

Status: SPINE.6B operational wave contract.

## Completed Foundation

NEW.0 through NEW.19 are complete foundation history. They are no longer the
future scheduling system. SPINE.6A compressed active engineering docs. SPINE.6B
adds the operational extraction contract for future implementation waves.

The active roadmap now starts at:

```text
SPINE.20 Local Runtime Layout
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

Future implementation waves must classify corresponding old-yai residue. A
wave is not complete until old material has been absorbed, rewritten, split,
externalized, quarantined, marked compatibility-only, marked delete-later, left
untouched with a reason, or explicitly deferred.

Archived historical records live under:

```text
docs/archive/engineering/
```
