# Data Spine Refactor Roadmap

Status: SPINE.1 canonical refoundation roadmap, refined by NEW.13 planning.

YAI Core is residue-first. The current data implementation is split between C
and Rust as a bootstrap proof. That split works for NEW.0 through NEW.12, but
the mature architecture makes Rust the operational data spine and keeps C as
the system boundary.

## Data-Spine Doctrine

```text
YAI Core is residue-first.
Store is not memory.
Graph is not lineage.
Index/query is not RAG.
Memory is not chat history.
Projection is not UI state.
Reconcile is not recovery execution.
```

Definitions:

| Primitive | Definition |
|---|---|
| store | durable residue persistence |
| graph | causality and reconstruction over residue |
| index/query | operational access over case-bound residue |
| memory | receipt-backed, graph-derived, policy-aware operational experience |
| projection | controlled read model for model, agent, operator, API, audit and debug |
| reconcile | expected-vs-observed mismatch detection and recovery posture |

## Memory Doctrine

| Term | Meaning |
|---|---|
| RAG | retrieves documents or knowledge |
| Agent memory | remembers textual or task experience |
| YAI memory | remembers decisions, receipts, subject state, policy, graph paths and divergences |

YAI memory must always be:

```text
case-bound
subject-aware
policy-aware
receipt-backed
graph-derived
scoped
freshness-aware
projection-controlled
```

## C And Rust Doctrine

C owns:

```text
public ABI
daemon bootstrap
host carriers
control enforcement boundary
system bridge
FFI shims
```

Rust owns:

```text
store
journal
record codec
graph
index/query
memory
projection
reconcile
retention
integrity
```

Target:

```text
C wrappers become thin.
C duplicate data logic is quarantined and eventually removed.
Rust engine becomes the operational data spine.
```

## Refactor Order

Completed:

```text
NEW.0    skeleton + guards
NEW.1    minimum loop
NEW.2    persistent journal + subject state
NEW.3    control gate + policy skeleton
NEW.4    sandboxed filesystem carrier
NEW.5    graph reconstruction
NEW.5A   smoke isolation
SPINE.0  wave-coupled extraction
NEW.6    operational memory candidate
NEW.7    reconcile/divergence
NEW.8    projection hardening
NEW.9    query boundary
NEW.10   Rust engine R1
NEW.11   daemon IPC
NEW.12   daemon-backed core loop
```

Filesystem / engine refoundation:

```text
NEW.13  target filesystem doctrine: system/engine/cmd/include
NEW.14  move Rust engine workspace: crates/yai-core-engine* -> engine/
NEW.15  move yai command: crates/yai-ctl -> cmd/yai
NEW.16  move yaid entrypoint: daemon/main.c -> cmd/yaid/main.c
NEW.17  move C implementation: lib/ -> system/
NEW.18  split C data logic: system bridge vs engine ownership
NEW.19  Makefile/build/guards realignment
NEW.20  local install layout: yai + yaid
NEW.21  filesystem refactor milestone freeze
```

NEW.14 status: completed as the first physical refactor wave. It moved
`crates/yai-core-engine` to `engine/yai-engine` and
`crates/yai-core-engine-sys` to `engine/yai-engine-ffi`.

NEW.15 status: completed as the command move wave. It moved `crates/yai-ctl` to
`cmd/yai` and removed `crates/`.

NEW.16 status: completed as the daemon move wave. It moved `daemon/main.c` to
`cmd/yaid/main.c`, daemon support to `system/daemon`, and removed the top-level
`daemon/` source root.

NEW.17 status: completed as the C system move wave. It moved the remaining C
implementation from `lib/` to `system/`, removed `lib/` and removed the retired
`ctl/` pointer root. The data-spine C folders under
`system/{store,graph,index,memory,projection,reconcile}` remain transitional
until NEW.18.

NEW.13 planning artifacts:

```text
new13-filesystem-refactor-plan.md
filesystem-move-matrix.md
engine-ownership-map.md
system-ownership-map.md
new15-command-layout.md
new16-daemon-layout.md
new17-system-layout.md
```

Model/provider experiment runway:

```text
NEW.22  ingest material model v0
NEW.23  model/provider subject posture v0
NEW.24  model output claim import v0
NEW.25  policy pack skeleton + model projection v0
NEW.26  naked local model case experiment v0
NEW.27  model behavior trace v0
NEW.28  model carrier v0
NEW.29  agent trace/tool call import v0
NEW.30  policy compliance experiment harness v0
```

The first real naked model test is NEW.26. The first core-owned model
invocation is NEW.28. The first agent-framework test is NEW.29/NEW.30.
Provider scouting may begin immediately at L0, outside the core, but it is not
canonical core validation.

The experiment maturity doctrine is maintained in
`model-provider-experiment-ladder.md`. The NEW.26 test shape is maintained in
`naked-model-case-test-plan.md`.

The later roadmap is maintained in `four-repo-roadmap.md`.

## Old-YAI Audit Scope

Filesystem and data-spine refactor waves must inspect the old `yai` concept
mine when their concepts overlap:

```text
../yai/src/substrate/store/*
../yai/src/substrate/records/*
../yai/src/substrate/graph/*
../yai/src/substrate/indexes/*
../yai/src/substrate/query/*
../yai/src/substrate/memory/*
../yai/src/substrate/views/*
../yai/src/substrate/signals/*
../yai/src/lineage/*
../yai/src/analytics/*
../yai/src/models/*
../yai/src/agents/grounding/*
../yai/src/capabilities/mcp/*
../yai/src/capabilities/external/*
../yai/src/runtime/provider/*
../yai/src/runtime/execution/*
../yai/src/runtime/carriers/*
../yai/src/runtime/machine/*
../yai/src/runtime/lifecycle/*
../yai/src/case/policy/*
../yai/src/case/subjects/*
../yai/src/case/materialization/*
../yai/src/case/surface/*
```

Future model waves classify the additional old-yai material as:

| Old-yai path | Classification |
|---|---|
| `src/models/*` | future model subject/provider posture, ai-environment and model carrier concepts |
| `src/agents/grounding/*` | future ai-environment and model projection experiments |
| `src/runtime/provider/*` | future model carrier/provider subject concepts |
| `src/capabilities/mcp/*` | future agent/tool adapter tests |
| `src/capabilities/external/*` | future external tool/provider adapter tests |
| `src/substrate/views/*` | projection concepts |
| `src/substrate/memory/*` | memory concepts |
| `src/case/policy/*` | policy materialization and model projection evidence |

In NEW.13 there is no code extraction, source movement or file migration.

## SPINE.2 Non-Goals

SPINE.2 must not:

```text
implement model runner
implement provider registry
create model carrier
create policy packs
create ai-environment repo
touch old yai
touch interfaces
touch console
```

## Stub And Incomplete Rule

When a refactor wave touches a stub, TODO-only file, placeholder, bootstrap
bridge or half-implemented surface, it must:

```text
verticalize it into the new doctrine
or quarantine it
or mark delete_later
```

It must not leave stale bootstrap language alive.
