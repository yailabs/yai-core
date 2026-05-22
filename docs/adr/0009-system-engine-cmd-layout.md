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

`lib/`, retired `ctl/` and top-level `daemon/` are transitional bootstrap
roots. `crates/` is removed after NEW.15. They must not be treated as final
layout doctrine.

## Consequences

The next implementation wave is NEW.13 target filesystem doctrine/refactor
plan. Local install layout is delayed to NEW.20 so the project does not install
a filesystem shape already known to be transitional.

NEW.13 defines the exact staged sequence:

```text
NEW.14  move Rust workspace: crates/ -> engine/
NEW.15  move yai command: crates/yai-ctl -> cmd/yai
NEW.16  move yaid entrypoint: daemon/main.c -> cmd/yaid/main.c
NEW.17  move C implementation: lib/ -> system/
NEW.18  split C data logic: system bridge vs engine ownership
NEW.19  Makefile/build/guards realignment
NEW.20  local install layout: yai + yaid
NEW.21  filesystem refactor milestone freeze
```

NEW.15 makes `cmd/yai` the canonical Rust technical command location. It does
not make `yai-core` own Console UX.

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
