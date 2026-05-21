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
NEW.14  move Rust workspace: crates/ -> engine/
NEW.15  move yai command: crates/yai-ctl -> cmd/yai
NEW.16  move yaid entrypoint: daemon/main.c -> cmd/yaid/main.c
NEW.17  move C implementation: lib/ -> system/
NEW.18  split C data logic: system bridge vs engine ownership
NEW.19  Makefile/build/guards realignment
NEW.20  local install layout: yai + yaid
NEW.21  filesystem refactor milestone freeze
```

NEW.13 planning artifacts:

```text
new13-filesystem-refactor-plan.md
filesystem-move-matrix.md
engine-ownership-map.md
system-ownership-map.md
```

Data spine Rust maturation:

```text
NEW.22  Rust store backend R2
NEW.23  Rust record codec / journal R2
NEW.24  Rust query/index R2
NEW.25  Rust graph reconstruction R2
NEW.26  Rust memory consolidation R2
NEW.27  Rust projection materialization R2
NEW.28  Rust reconcile detection R2
NEW.29  C wrapper thinning
NEW.30  data spine milestone freeze
```

The later NEW.31 through NEW.63 roadmap is maintained in
`four-repo-roadmap.md`.

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
../yai/src/agents/grounding/*
../yai/src/runtime/execution/*
../yai/src/runtime/carriers/*
../yai/src/runtime/machine/*
../yai/src/runtime/lifecycle/*
../yai/src/case/subjects/*
../yai/src/case/materialization/*
../yai/src/case/surface/*
```

In NEW.13 there is no code extraction, source movement or file migration.

## Stub And Incomplete Rule

When a refactor wave touches a stub, TODO-only file, placeholder, bootstrap
bridge or half-implemented surface, it must:

```text
verticalize it into the new doctrine
or quarantine it
or mark delete_later
```

It must not leave stale bootstrap language alive.
