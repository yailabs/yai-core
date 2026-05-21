# ADR 0011: Lib Is Transitional

## Status

Accepted for SPINE.1.

## Context

The current `lib/` root contains the C implementation for the bootstrap loop
and still includes store, graph, index, memory, projection and reconcile
surfaces. That helped NEW.0 through NEW.12 converge, but it should not be read
as the mature implementation root.

## Decision

`lib/` is transitional. The long-term C implementation root is `system/`, and
Rust-owned data logic belongs under `engine/`.

Current bootstrap roots are classified as:

```text
lib/              transitional
crates/           transitional
ctl/              transitional
top-level daemon/ transitional
```

## Consequences

Future refactor waves must split system-boundary C code from operational data
engine ownership. Any touched stub, TODO-only file, placeholder, bootstrap
bridge or half-implemented surface must be verticalized into the new doctrine,
quarantined or marked `delete_later`.

NEW.13 records the exact split:

```text
lib/base        -> system/base
lib/case        -> system/case
lib/subject     -> system/subject
lib/op          -> system/op
lib/control     -> system/control
lib/effect      -> system/effect
lib/daemon      -> system/daemon
lib/internal    -> system/internal

lib/store       -> system/engine_bridge + engine/yai-engine/src/store
lib/graph       -> system/engine_bridge + engine/yai-engine/src/graph
lib/index       -> system/engine_bridge + engine/yai-engine/src/index/query
lib/memory      -> system/engine_bridge + engine/yai-engine/src/memory
lib/projection  -> system/engine_bridge + engine/yai-engine/src/projection
lib/reconcile   -> system/engine_bridge + engine/yai-engine/src/reconcile
```

## Non-goals

Do not delete or move `lib/` in SPINE.1. Do not use this decision as permission
to rewrite C data code before the scheduled refactor waves.

## Related docs

```text
../engineering/filesystem-target-v2.md
../engineering/target-layout.md
../engineering/data-spine-refactor-roadmap.md
../engineering/new13-filesystem-refactor-plan.md
../engineering/system-ownership-map.md
```
