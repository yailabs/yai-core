# Architecture

This folder defines the canonical architecture of the future YAI Core.

It must stay technical and operational. Commercial positioning belongs in
`../product/`. Extraction details and filesystem plans belong in
`../engineering/`.

## Canonical Roots

SPINE.1 filesystem target roots:

```text
include
system
engine
cmd
proto
tests
docs
tools
packaging
examples
vendor
```

Primitive responsibilities inside `system/`, `engine/` and `include/`:

```text
base
ingest
subject
case
op
control
effect
store
index
graph
memory
projection
reconcile
daemon
ctl
```

`lib/` and retired `ctl/` are transitional bootstrap
roots, not the final filesystem doctrine.

## First Read

```text
00-spine.md
01-terminology.md
02-integration-modes.md
04-subject-model.md
06-control-policy-model.md
repo-split.md
```

## Non-Goals

YAI Core is not an agent framework, workflow engine, TUI, assistant, cloud platform, compliance certificate, or replacement for existing AI stacks.
