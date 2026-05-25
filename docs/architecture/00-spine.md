# YAI Spine

Status: SPINE.4 canonical spine.

YAI is a local AI operational control core.

It builds case worlds, binds operational subjects to cases, captures operation
attempts, materializes policy into machine gates, emits control decisions,
executes or observes effects through carriers, records receipts, derives
operational memory and serves live controlled projections.

SPINE.4 assumes NEW.18B is done. Case-world material comes before subject
behavior, active runtime operates through case sessions and contexts, projection
is a live versioned cognitive view, and operational observability/evaluation is
now a first-class plane.

Core truth is residue-based. YAI owns operational truth only through
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
| Interaction thread | [03C-interaction-thread-model.md](03C-interaction-thread-model.md) |
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
| Pack materialization | [15-pack-materialization.md](15-pack-materialization.md) |
| Hot state plane | [16-hot-state-plane.md](16-hot-state-plane.md) |
| LMDB record plane | [17-lmdb-record-plane.md](17-lmdb-record-plane.md) |
| Active roadmap | [../engineering/four-repo-roadmap.md](../engineering/four-repo-roadmap.md) |
| Current status | [../engineering/current-status.md](../engineering/current-status.md) |
| Filesystem target | [../engineering/filesystem-target.md](../engineering/filesystem-target.md) |
| Data plane roadmap | [../engineering/data-plane-roadmap.md](../engineering/data-plane-roadmap.md) |
| Wave template | [../engineering/wave-template.md](../engineering/wave-template.md) |

## Machine Spine

```text
ingest
-> pack inspection / validation
-> pack materialization
-> case_world
-> case_domain
-> case_attachment
-> case_binding
-> case_session
-> case_context
-> active_interaction_thread
-> participant_view_frame
-> subject
-> policy_rule
-> projection_rule
-> authority_scope
-> live_projection
-> hot_state freshness / invalidation
-> interaction_turn
-> model_interpretation / claim / attempt
-> operation attempt
-> control / policy gate
-> effect / carrier
-> receipt
-> journal
-> record store
-> LMDB durable indexed record lookup
-> graph
-> index/query
-> retrieval units / HNSW candidates
-> context compiler
-> compiled projection / model context
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

Pack rule:

```text
A pack is not operational until its material has been inspected, validated,
materialized and bound as case-world residue.
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
model/provider implementation follows the SPINE.23-SPINE.80 data-plane foundation
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

## Context Compiler / Retrieval Doctrine

Projection does not disappear. Projection evolves into the controlled view
produced by the context compiler: scoped, redacted, freshness-aware and
consumer-aware.

The Context Compiler is the model-neutral YAI boundary that assembles a
projection/model context from hot state, LMDB records, Ladybug graph, DuckDB
facts, operational memory, policy/authority and retrieval candidates. A
compiled model context is not truth, not memory, not the record plane and not
the graph.

ContextFrame may survive as an implementation artifact, but it is a temporary
compiled artifact and not durable truth.

HNSW is candidate retrieval. HNSW is not graph, not memory, not decision
authority and not policy authority. HNSW is a rebuildable proximity index over
retrieval units. The source of truth remains records, graph, facts, receipts,
policy and memory basis.

Canonical rule:

```text
A YAI decision may use HNSW/vector-retrieval candidates,
but no YAI decision may be justified by vector proximity alone.
```

HNSW finds. Case scope filters. Policy validates. Evidence justifies.
Record/graph/facts provide basis. The Context Compiler assembles the controlled
view. Memory consolidates only if authorized.

## Model Runtime / Runner Doctrine

YAI may own model runner contracts, invocation envelopes, receipts, local runner
adapters and specialized runtime harnesses. YAI must not immediately reinvent
llama.cpp, Ollama, MLX, vLLM, DS4 or every GGUF/local-model runtime.

Inference engines load models, run tokens and manage KV/cache/model-specific
execution. YAI owns case binding, projection, authority, prompt/invocation
envelopes, model carriers, provider/runner subjects, receipts, model output
import, behavior facts and memory/reconcile consequences. Runner output is not
case authority.

MTP belongs to model runtime / runner / decoding strategy. MTP is not
retrieval, not policy authority and not case truth.

```text
HNSW reduces input/context selection cost.
MTP reduces output token generation latency.
```

YAI may track supports_mtp, mtp_mode, mtp_depth, requires_mtp_weights,
requires_draft_model, backend_support, acceptance_rate, fallback_mode,
structured_output_risk, latency_delta and tokens_per_second_delta. MTP may
accelerate generation; it must not change output authority.

## Core Invariants

```text
no operation without a case boundary
no participant without case-world material
no active operation without case context
no model-visible prompt without selected thread and participant view frame
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
| `pack` | materialization units for methods, policies, projections, templates, memory seeds and fixtures | marketplace, billing, installer, runtime execution or product catalog |
| `case_world` | domains, attachments, bindings and authority/projection posture | execution authority by attachment alone |
| `subject` | operational entities bound to case context | full external system state |
| `case` | boundary, posture, case-world material, case sessions, context, subject binding, evidence refs | abstract world governance |
| `interaction_thread` | selected conversational lane and participant view frame posture | audit truth or whole case projection |
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

YAI does not convert claims into receipts by wording. Claims become
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
YAI is residue-first.
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

NEW.18C interaction-thread doctrine:

```text
journal = replay/audit, not active conversation memory
interaction_thread = selected conversational lane
participant_view_frame = model-visible assembled context for a turn
transcript = audit material, not automatically model context
```

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

SPINE.33L provider runtime posture:

```text
device -> provider supervisor -> provider instance -> model attachment -> case
```

Runtime targets are `local`, `lan` and `external`. SPINE.33L exposes device
registry, provider start dry-run plans, provider logs, model catalog posture,
model routing, context compiler, retrieval_hnsw and decoding acceleration
status. It starts no provider and performs no remote start in SPINE.33L.

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

YAI must never overclaim control.

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
yai       = canonical local AI operational control system
yai-dev   = development lab, concept mine, harness and scenario workspace
interfaces= contracts/API/SDK/conformance
studio    = official product client surface
console   = operator terminal/client UX
```

`yai` is a workspace sibling to `yai-dev`, not nested inside it. `yai-dev`
does not own core truth; it provides material to extract, rewrite, classify or
externalize through explicit SPINE waves.

## Implementation Order

```text
DOC.CORE.0: canonical docs freeze
NEW.1: minimum loop implementation
NEW.13: target filesystem doctrine: system/engine/cmd/include
NEW.18A: case world binding records
NEW.18B: live case context / ref boundary
SPINE.3R: case world + live data plane rebase
NEW.19: Makefile/build/guards realignment
SPINE.20: local runtime layout
SPINE.21: pack materialization doctrine
SPINE.23-SPINE.80: hot, record, graph, fact, projection, memory, reconcile and observability data planes
SPINE.28A: repository identity cutover to yai + yai-dev
SPINE.28B: internal source surface cleanup
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

YAI is not an agent framework, workflow engine, product CLI, TUI, general
assistant, cloud platform, model provider, SDK surface, compliance certificate
or dashboard.

Do not migrate old roots as folders. Do not make Console or Interfaces the
product root for core truth.

SPINE.3R must not implement a model runner, provider registry, model carrier,
policy packs, backend databases or shared memory. Future waves must not touch
`yai-dev`, `interfaces`, `studio` or `console` unless explicitly scoped.
