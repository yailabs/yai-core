# ADR 0000: Decision Compendium

## Status

Accepted for DOC.CORE.0.

## Context

DOC.NEW.0 created the first canonical documentation package. DOC.NEW.1 deepened
that package into machine contracts. DOC.CORE.0 freezes the documentation before
NEW.1 implementation.

The numbered ADR files remain the atomic decision records. This compendium is a
fast-path index and doctrine view. It does not replace the atomic ADRs.

## Decision

Maintain this file as the fast-path index for the current ADR set while keeping
the numbered ADRs as the atomic records.

SPINE.0 adds the four-repo doctrine: `yai-core` is the canonical new core,
old `yai` is transition concept mine and future `ai-environment` material,
`interfaces` projects primitive truth through API/SDK/conformance, and `console`
is an operator client over projections and interfaces.

SPINE.0 also adopts wave-coupled extraction. Old `yai` is not ignored until a
final cleanup wave. Each `yai-core` implementation wave that overlaps an old
concept must audit the corresponding old material, extract concepts only,
implement the new primitive in the new grammar, update the extraction inventory
and classify residue.

SPINE.1 adopts filesystem / data-spine refoundation as the next phase. The
target layout is `include/`, `system/`, `engine/` and `cmd/`; Rust owns the
operational data spine. `crates/` is removed after NEW.15, top-level `daemon/`
after NEW.16, and `lib/` plus retired `ctl/` after NEW.17.
NEW.18 creates `system/engine_bridge` as the active C bridge into Rust and
classifies remaining C data logic as temporary smoke support.

SPINE.2 adopts the model/provider experiment maturity ladder. L0 provider
scouting can start immediately outside the core. The first canonical AI
behavior test is a naked model inside a case at NEW.26, not agent framework
integration. Core-owned model invocation waits for NEW.28 model carrier v0.
Agent framework/tool-call traces begin after that at NEW.29/NEW.30.

## Decision Set

| ADR | Decision | Effect |
|---|---|---|
| 0001 | New core boundary | The core is not a cleaned-up copy of legacy `src/`. |
| 0002 | Case-bound subjects | Subjects become first-class primitives bound to cases. |
| 0003 | Daemon over runtime | `yaid` and `daemon` replace the old `runtime` root model. |
| 0004 | Control over decision root | `control` owns policy, gates, admission, decision and obligations. |
| 0005 | Store/index/graph/memory/projection split | The old `substrate` root is split into explicit data roots. |
| 0006 | No agent framework in core | Agents integrate with core; they do not define core architecture. |
| 0007 | No workflow engine in core | Workflows are subjects/events/procedure records, not a core engine. |
| 0008 | C first, Rust operational data engine | C owns public contracts; Rust later owns advanced operational data. |
| 0009 | System engine cmd layout | `include/`, `system/`, `engine/` and `cmd/` are the target layout. |
| 0010 | Rust data spine ownership | Rust owns store, journal, record, graph, index/query, memory, projection, reconcile, retention and integrity. |
| 0011 | Lib is transitional | `lib/` and retired `ctl/` were bootstrap roots and are removed after NEW.17. |
| 0012 | Naked model before agent framework | First AI behavior experiment is a naked model inside a case; agent frameworks are later adapters. |

## Combined Doctrine

The future core starts from a small machine spine:

```text
case binds subjects
ops target subjects
control gates ops through materialized policy
effects happen through carriers or observation
receipts persist what happened
store records truth
graph reconstructs relationships
memory derives scoped operational experience
projection serves controlled read models
reconcile exposes mismatch
```

Core truth exists through case-bound records, subject bindings, attempts,
decisions, receipts, graph relations, memory and projections. The legacy `yai`
repository is a concept mine. The `yai-core` repository is shaped by primitives,
protocols and receipts, not by old folder names.

Four-repo ownership:

```text
yai-core   owns core primitives and local operational control
yai        remains transition concept mine until ai-environment canonicalization
interfaces consumes yai-core truth and publishes schemas, SDK and conformance
console    consumes projections and interfaces for human operator UX
```

Canonical wave families:

```text
CORE.NEW.*
ENV.CANON.*
INTF.CANON.*
CONSOLE.CANON.*
PLATFORM.LATER.*
```

Wave-coupled extraction keeps old `yai` useful without allowing old roots to
re-grow inside `yai-core`. Useful non-core material is assigned to future
`ai-environment` with `future_repo=ai-environment` and `action=externalize`.

Data-spine doctrine:

```text
YAI Core is residue-first.
Store is not memory.
Graph is not lineage.
Index/query is not RAG.
Memory is not chat history.
Projection is not UI state.
Reconcile is not recovery execution.
```

Model/provider experiment doctrine:

```text
L0 provider scouting can start immediately outside the core
NEW.26 is the first real naked model case experiment
NEW.28 is the first core-owned model invocation
NEW.29/NEW.30 are the first agent-framework/tool-call test path
```

A model is not inside the core. It is a case-bound subject with locality,
provider, posture and observed behavior. Model output enters through projection,
claim or observation, control decision, receipt and memory. Policy is
materialized cognitively through projection and operationally through gates,
decision and carrier.

C/Rust target:

```text
C owns public ABI, daemon bootstrap, host carriers, control enforcement
boundary, system bridge and FFI shims.
Rust owns store, journal, record codec, graph, index/query, memory, projection,
reconcile, retention and integrity.
```

## Consequences

Readers can understand the decision set quickly without losing atomic ADR
ownership. Changes to a decision must still update the corresponding numbered
ADR first, then this compendium.

## Non-goals

```text
do not use this compendium as the only ADR record
do not implement NEW.1 from this file alone
do not treat the old src roots as migration targets
do not rename yai during SPINE.0
do not let interfaces or console define core semantics independently
do not postpone all old-yai residue handling to a final cleanup pass
do not begin core AI behavior validation with an agent framework
do not turn L0 provider scouting into canonical core validation
```

## Supersedes

No atomic ADR. This file supersedes ad hoc reading order notes for the ADR set.

## Related docs

```text
../architecture/00-spine.md
../architecture/04-subject-model.md
../architecture/06-control-policy-model.md
../engineering/target-layout.md
../engineering/extraction-plan.md
../engineering/four-repo-roadmap.md
../engineering/filesystem-target-v2.md
../engineering/data-spine-refactor-roadmap.md
../engineering/model-provider-experiment-ladder.md
../engineering/naked-model-case-test-plan.md
```
