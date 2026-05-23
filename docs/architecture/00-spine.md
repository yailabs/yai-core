# YAI Core Spine

Status: SPINE.4 canonical spine.

YAI Core is a local AI operational control core.

It builds case worlds, binds operational subjects to cases, captures operation
attempts, materializes policy into machine gates, emits control decisions,
executes or observes effects through carriers, records receipts, derives
operational memory and serves live controlled projections.

SPINE.4 assumes NEW.18B is done. Case-world material comes before subject
behavior, active runtime operates through case sessions and contexts, projection
is a live versioned cognitive view, and operational observability/evaluation is
now a first-class plane.

Core truth is residue-based. YAI Core owns operational truth only through
case-bound records, case-world material, subject bindings, attempts, decisions,
receipts, graph relations, memory and projections. It does not own external
system truth unless the external system binds or reports through YAI protocols.

## Reading Map

| Need | Read |
|---|---|
| Frozen terms | [01-terminology.md](01-terminology.md) |
| Integration guarantees | [02-integration-modes.md](02-integration-modes.md) |
| Case boundary | [03-case-domain.md](03-case-domain.md) |
| Case world | [03A-case-world-model.md](03A-case-world-model.md) |
| Live case context | [03B-live-case-context.md](03B-live-case-context.md) |
| Subjects | [04-subject-model.md](04-subject-model.md) |
| Operations | [05-operation-model.md](05-operation-model.md) |
| Control and policy | [06-control-policy-model.md](06-control-policy-model.md) |
| Carriers and receipts | [07-effect-carrier-model.md](07-effect-carrier-model.md) |
| Store/index/graph | [08-store-index-graph-model.md](08-store-index-graph-model.md) |
| Memory | [09-memory-model.md](09-memory-model.md) |
| Projection | [10-projection-model.md](10-projection-model.md) |
| Reconciliation | [11-reconciliation-model.md](11-reconciliation-model.md) |
| Daemon and ctl | [12-daemon-and-ctl.md](12-daemon-and-ctl.md) |
| C/Rust boundary | [13-c-rust-boundary.md](13-c-rust-boundary.md) |
| Observability and evaluation | [14-operational-observability-evaluation.md](14-operational-observability-evaluation.md) |
| Case-world verticalization | [../engineering/case-world-verticalization.md](../engineering/case-world-verticalization.md) |
| Operational data planes | [../engineering/operational-data-plane-doctrine.md](../engineering/operational-data-plane-doctrine.md) |
| Live projection | [../engineering/live-projection-model.md](../engineering/live-projection-model.md) |
| Case View Quality | [../engineering/case-view-quality.md](../engineering/case-view-quality.md) |
| Target filesystem V2 | [../engineering/filesystem-target-v2.md](../engineering/filesystem-target-v2.md) |
| Data spine refactor | [../engineering/data-spine-refactor-roadmap.md](../engineering/data-spine-refactor-roadmap.md) |
| Model/provider experiment ladder | [../engineering/model-provider-experiment-ladder.md](../engineering/model-provider-experiment-ladder.md) |
| Naked model case test plan | [../engineering/naked-model-case-test-plan.md](../engineering/naked-model-case-test-plan.md) |

## Machine Spine

```text
ingest
-> case_world
-> case_domain
-> case_attachment
-> case_binding
-> case_session
-> case_context
-> subject
-> policy_rule
-> projection_rule
-> authority_scope
-> live_projection
-> model_interpretation / claim / attempt
-> operation attempt
-> control / policy gate
-> effect / carrier
-> receipt
-> journal
-> record store
-> graph
-> index/query
-> memory
-> reconcile
-> observability / evaluation
-> next projection delta
```

The system starts from case-world material, attempts, policy, effects,
receipts, operational memory and live controlled projections. It does not start
from agents, workflows, prompts, UI or model providers.

Case-world rule:

```text
Everything that participates in a case enters first as case-world material:
a domain, attachment or binding.
```

Live-case rule:

```text
case_ref is not the live case.
subject_ref is not the live subject.
receipt_ref is not the receipt state.
refs cross boundaries; contexts operate.
```

## Model Provider Experiment Doctrine

SPINE.2 defines when model/provider experiments become canonical:

```text
L0 provider scouting can start immediately outside the core
model/provider implementation follows the NEW.22-NEW.30 data-plane foundation
```

A model is not inside the core. A model is a case-bound subject with locality,
provider, posture and observed behavior. The first AI behavior experiment is a
naked model inside a case, not agent framework integration.

Model invocation and output enter core truth through:

```text
projection
claim or observation
control decision
receipt
memory
```

Policy is materialized twice:

```text
cognitively through projection
operationally through gates, decision and carrier
```

Agent frameworks are not first. Agents become later subjects, actors or sources
through external environment/adapters after naked model behavior is measurable.

## Core Invariants

```text
no operation without a case boundary
no participant without case-world material
no active operation without case context
no target without a subject reference
no enforcement claim without a control decision
no effect truth without receipt or observation provenance
no memory without record, receipt or graph basis
no projection without provenance, freshness, version and redaction posture
no reconciliation without expected-vs-observed comparison
no controlled action without knowable-enough case view quality
```

These invariants are stronger than folder layout. If an implementation violates
them, it is outside the core doctrine even if it uses the right names.

## Primitive Ownership

| Primitive | Owns | Does not own |
|---|---|---|
| `base` | ids, time, status, errors, byte refs | domain semantics |
| `ingest` | normalization of material and claims | belief or execution |
| `case_world` | domains, attachments, bindings and authority/projection posture | execution authority by attachment alone |
| `subject` | operational entities bound to case context | full external system state |
| `case` | boundary, posture, case-world material, case sessions, context, subject binding, evidence refs | abstract world governance |
| `op` | normalized attempted operation | policy decision or execution |
| `control` | policy materialization, gates, decisions, obligations | carrier side effects |
| `effect` | carrier requests, execution/observation, receipts | policy authority |
| `store` | durable records, journals, blobs, integrity | intelligent recall |
| `index` | retrieval structures and query access | truth ownership |
| `graph` | relationships, causality, reconstruction paths | raw record persistence |
| `memory` | scoped operational experience from residue | hidden prompt context |
| `projection` | controlled read models | source-of-truth state |
| `reconcile` | mismatch detection and recovery posture | silent repair |
| `observability` | diagnostic, temporal, epistemic and behavioral facts over residue | case truth or model inference |
| `daemon` | local residency, IPC, loop, supervision | product UX |
| `ctl` | technical core command surface | console experience or semantics |

## Operational Truth Model

| Level | Meaning | Example |
|---|---|---|
| claim | unverified material entered the system | external tool says it wrote a file |
| observation | YAI observed material or state | filesystem state changed |
| decision | YAI evaluated an attempt | write allowed with constraints |
| receipt | effect or observation was recorded with provenance | carrier receipt for file write |
| reconstruction | records and graph edges explain sequence | decision -> carrier -> receipt |
| memory | residue has been consolidated under scope | repo write pattern remembered for case |
| projection | live controlled view is served to a consumer | audit packet or model context |

YAI Core does not convert claims into receipts by wording. Claims become
evidence only through ingest, binding, provenance and store records.

## Evidence, Receipt And Memory

Evidence is any case-bound record or reference that can support a decision,
reconstruction or projection.

A receipt is durable evidence connecting an attempted operation, decision
posture, carrier or observation path, affected subjects and result status.
Imported receipts keep external provenance and weaker guarantee labels.

Memory is scoped operational experience derived from records, receipts, graph
paths and decisions. It is not raw storage and not a context pack.

## Data Spine Doctrine

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
| projection | live versioned cognitive view for model, agent, operator, API, audit and debug |
| reconcile | expected-vs-observed mismatch detection and recovery posture |
| observability/evaluation | case-view quality, trace, freshness, provenance and behavior diagnostics |

SPINE.3R data-plane stratification:

| Plane | Role |
|---|---|
| hot | shared memory / hot state, not truth |
| journal | append-only replay chronology |
| record | durable normalized record lookup, LMDB or equivalent |
| graph | operational causality and reconstruction, Ladybug target |
| fact | derived analytical facts, DuckDB target |
| memory | operational memory over records, receipts, graph, policy and divergences |
| projection | live, versioned, delta-aware view materialization |
| reconcile | mismatch detection and recovery posture |

SPINE.4 observability/evaluation doctrine:

```text
YAI does not only control actions.
YAI measures whether the case remains knowable enough for controlled action.
```

Case View Quality, `CVQ`, is the canonical quality vector:

```text
freshness
causal_completeness
provenance_sufficiency
projection_consistency
authority_alignment
memory_basis_quality
divergence_exposure
delta_accuracy
cost
```

## Control Guarantee Model

YAI Core must never overclaim control.

| Mode | Core can claim | Core cannot claim |
|---|---|---|
| observed | observed, recorded, reconstructed | prevented, blocked or enforced |
| advisory | recommended or warned | blocked, enforced or executed |
| interposed | decided before forwarding effect request | real effect unless receipt returns |
| enforced | host/carrier is expected to honor decision | bypass impossible outside declared boundary |
| carrier_owned | executed or blocked through YAI carrier | external bypass prevention outside carrier |
| embedded | host-integrated decision and receipt path | control outside declared embed boundary |

The same primitives exist in every mode, but their guarantees differ. Projection
and product language must preserve the guarantee label.

## Repository Split

```text
yai-core   = canonical local AI operational control core
yai        = old/current concept mine, future ai-environment material
interfaces = projection/API/SDK/conformance
console    = operator terminal/client UX
```

`yai-core` is a workspace sibling to `yai`, not nested inside `yai`.

## Implementation Order

```text
DOC.CORE.0: canonical docs freeze
NEW.1: minimum loop implementation
NEW.13: target filesystem doctrine: system/engine/cmd/include
NEW.18A: case world binding records
NEW.18B: live case context / ref boundary
SPINE.3R: case world + live data plane rebase
NEW.19: Makefile/build/guards realignment
NEW.20: local install layout
NEW.22-NEW.30: hot, record, graph, fact, projection, memory and reconcile data planes
Rust operational data spine behind C FFI
```

NEW.1 proves:

```text
open case
build case_world
bind subject
submit op attempt
control decision
null carrier / observed receipt
append store record
query projection
```

## Non-Goals

YAI Core is not an agent framework, workflow engine, product CLI, TUI, general
assistant, cloud platform, model provider, SDK surface, compliance certificate
or dashboard.

Do not migrate old roots as folders. Do not make Console or Interfaces the
product root for core truth.

SPINE.3R must not implement a model runner, provider registry, model carrier,
policy packs, backend databases, shared memory or `ai-environment` repo. It
must not touch old `yai`, `interfaces` or `console`.
