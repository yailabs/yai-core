# Architecture

This folder defines the canonical architecture of the future YAI.

It must stay technical and operational. Commercial positioning belongs in
`../product/`. Extraction details and filesystem plans belong in
`../engineering/`.

## Canonical Roots

SPINE.3R filesystem and live data-plane target roots:

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
case_world
case_context
interaction_thread
observability
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
cmd
```

`lib/`, top-level `daemon/`, `crates/` and retired `ctl/` are bootstrap roots
removed by the NEW.14 through NEW.17 filesystem waves.

## First Read

```text
00-spine.md
01-terminology.md
02-integration-modes.md
03-case-domain.md
03A-case-world-model.md
03B-live-case-context.md
03C-interaction-thread-model.md
16-hot-state-plane.md
17-lmdb-record-plane.md
18-control-carrier-substrate.md
14-operational-observability-evaluation.md
04-subject-model.md
06-control-policy-model.md
diagrams/README.md
repo-split.md
```

`diagrams/` is the checkpoint diagram asset home. Diagrams are visual aids, not
implementation status.

## Non-Goals

YAI is not an agent framework, workflow engine, TUI, assistant, cloud platform, compliance certificate, or replacement for existing AI stacks.
