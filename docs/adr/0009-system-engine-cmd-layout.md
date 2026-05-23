# ADR 0009: System Engine Cmd Layout

## Status

Accepted for SPINE.1.

## Context

NEW.0 through NEW.12 proved the local core loop with a bootstrap tree:
`include/`, `lib/`, `daemon/`, `ctl/` and `crates/`. That tree works, but it
mixes system-boundary C code, Rust data-engine code and command entrypoints in
roots that are no longer the mature architecture.

## Decision

Adopt the target top-level shape:

```text
include/ = public C ABI contracts
system/  = C system plane: daemon, host, carriers, control boundary, FFI bridges
engine/  = Rust operational data engine
cmd/     = binaries: yai and yaid
proto/   = schemas and fixtures
tests/   = smoke, unit, integration and adversarial tests
```

`crates/`, top-level `daemon/`, `lib/` and retired `ctl/` are bootstrap roots.
`crates/` is removed after NEW.15, top-level `daemon/` after NEW.16, and
`lib/` plus `ctl/` after NEW.17. They must not be treated as final layout
doctrine.

## Consequences

At the time of this ADR, the next implementation wave was NEW.13 target
filesystem doctrine/refactor plan. That pre-SPINE.20 numbering is historical;
future scheduling now belongs to `docs/engineering/four-repo-roadmap.md`.

NEW.13 defines the exact staged sequence:

```text
NEW.14  move Rust workspace: crates/ -> engine/
NEW.15  move yai command: crates/yai-ctl -> cmd/yai
NEW.16  move yaid entrypoint: daemon/main.c -> cmd/yaid/main.c
NEW.17  move C implementation: lib/ -> system/
NEW.18  split C data logic: system bridge vs engine ownership
NEW.19  Makefile/build/guards realignment
SPINE.20  local install layout: yai + yaid
```

NEW.15 makes `cmd/yai` the canonical Rust technical command location. It does
not make `yai-core` own Console UX.
NEW.16 makes `cmd/yaid` the canonical C daemon entrypoint location and
`system/daemon` the daemon support location.
NEW.17 makes `system/` the active C implementation root. NEW.18 creates
`system/engine_bridge` and classifies data-spine C folders under
`system/{store,graph,index,memory,projection,reconcile}` as temporary smoke
support while Rust engine ownership matures.

## Non-goals

Do not move source files in SPINE.1. Do not rename repos. Do not create install
targets. Do not migrate old `yai` folders into `yai-core`.

## Related docs

```text
../engineering/filesystem-target-v2.md
../engineering/data-spine-refactor-roadmap.md
../engineering/target-layout.md
../engineering/new13-filesystem-refactor-plan.md
../engineering/filesystem-move-matrix.md
```
