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

SPINE.0 adds the four-repo doctrine: `yai` is the canonical new core,
`yai-dev` is transition concept mine and `yai-dev` lab material,
`interfaces` projects primitive truth through API/SDK/conformance, and `console`
is an operator client over projections and interfaces.

SPINE.0 also adopts wave-coupled extraction. `yai-dev` is not ignored until a
final cleanup wave. Each `yai` implementation wave that overlaps an old
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
scouting can start immediately outside the core. SPINE.3R later rebases the
roadmap so canonical model/provider implementation follows the NEW.22-NEW.30
data-plane foundation.

SPINE.3R adopts case-world-first operation and live data-plane doctrine.
Everything that participates in a case enters first as case-world material: a
domain, attachment or binding. Projection is not a summary; it is a versioned
cognitive view over the operational data planes of a case. The data planes are
hot/shared memory, journal, record, graph, fact, memory, projection and
reconcile. Shared memory, LMDB, Ladybug and DuckDB remain planned-not-created.

NEW.18B adds the live case context boundary. Refs identify durable material;
contexts operate on loaded case state. `case_ref`, `subject_ref` and receipt
refs remain in persisted residue and boundary messages, but active runtime work
uses `case_session` and `case_context`.

SPINE.4 adds the Operational Observability & Evaluation Plane. YAI does
not only control actions; it measures whether the case remains knowable enough
for controlled action. The canonical quality vector is Case View Quality
(`CVQ`).

NEW.18C adds the interaction thread / participant view boundary. Journal
remains replay/audit; active model context is selected interaction thread plus
participant view frame plus current case projection.

SPINE.23 adds hot state as the first live data plane. Hot state is not truth; it
is a daemon-owned live cache and snapshot used to expose active case/session,
projection freshness, stale reason and latest residue refs.

SPINE.29 defines LMDB as durable indexed record lookup. hot state is not LMDB.
journal remains replay/audit. LMDB is rebuildable from journal and does not own
graph, fact or memory semantics.

SPINE.30 adds the LMDB write path. The daemon loop still writes journal residue
first; `yai` mirrors completed loop journals into LMDB id/case/kind indexes and
fails explicitly if LMDB import fails.

SPINE.33G adopts non-process carrier skeletons. Planned carrier families must
be inspectable and receipt-aware before execution is implemented.

SPINE.33H adopts the carrier outcome harness. Outcome posture can be tested
across active, skeleton and unsupported families without executing skeleton
effects.

SPINE.33M adopts Data Context Runtime / RuntimeGraph doctrine. Truth lives on
durable planes and computation happens in runtime working sets. Ladybug stores
relations, RuntimeGraph computes over the active case, HNSW finds candidate
nodes and Context Compiler renders controlled views.

SPINE.34 adopts LMDB Record Plane Freeze. `yai.record.v1`, id/case/kind/
subject/receipt indexes and no journal fallback become the stable record-plane
posture for journal replay and graph persistence work.

SPINE.35 adopts Journal Replay Doctrine + Parser Hardening. Journal is
replay/audit, LMDB is durable indexed record lookup, replay diagnostics are
mandatory and `yai journal inspect` reports readiness without LMDB writes.

SPINE.36 adopts Journal Replay to LMDB. Replay materializes valid journal
residue into the LMDB record plane through the frozen writer, reports
records_written and records_duplicate, and keeps no journal fallback semantics.

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
| 0013 | Case world before subject binding | Case-world material precedes subject behavior. |
| 0014 | Operational data plane stratification | Hot, journal, record, graph, fact, memory, projection and reconcile planes are distinct. |
| 0015 | Projection as live cognitive view | Projection is live, versioned and delta-aware, not a static summary. |
| 0016 | Operational observability evaluation plane | Case-view quality, trace, freshness, provenance, behavior and memory quality are first-class. |
| 0017 | Packs as case materialization units | Packs materialize methods, policies, projections, schemas, memory seeds and fixtures into cases. |
| 0018 | Hot state is not truth | Hot state is live cache/freshness posture, not durable truth. |
| 0019 | LMDB record plane | LMDB is durable indexed record lookup, rebuildable from journal and separate from hot state, graph, facts and memory. |
| 0026 | Non-process carrier skeletons | Planned carrier families become carrier.v1 skeletons before implementation and do not execute effects. |
| 0027 | Carrier outcome harness | Outcome posture is tested independently of real carrier execution; skeleton carriers simulate posture only. |
| 0031 | Data Context Runtime / RuntimeGraph | Durable graph truth is separated from runtime graph computation, HNSW candidate retrieval and compiled projection output. |
| 0032 | LMDB record plane freeze | `yai.record.v1` id/case/kind/subject/receipt lookup is frozen with no journal fallback. |
| 0033 | Journal replay boundary | Journal inspect reports invalid_json, invalid_schema, unsupported_kind, duplicate and replay readiness without writing LMDB. |
| 0034 | Journal replay to LMDB | Journal replay writes valid records through the LMDB record/index path and treats existing records as idempotent duplicates. |

## Combined Doctrine

The future core starts from a small machine spine:

```text
case builds case_world
case opens case_session and derives case_context
case selects active_interaction_thread
participant_view_frame assembles model-visible context
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
observability/evaluation measures case-view quality
```

Core truth exists through case-bound records, subject bindings, attempts,
decisions, receipts, graph relations, memory and projections. Case-world
material precedes subject behavior. The legacy `yai` repository is a concept
mine. The `yai` repository is shaped by primitives, protocols and
receipts, not by old folder names.

Four-repo ownership:

```text
yai   owns core primitives and local operational control
yai        remains transition concept mine until yai-dev canonicalization
interfaces consumes yai truth and publishes schemas, SDK and conformance
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

Wave-coupled extraction keeps `yai-dev` useful without allowing old roots to
re-grow inside `yai`. Useful non-core material is assigned to future
`yai-dev` with `future_repo=yai-dev` and `action=externalize`.

Data-spine doctrine:

```text
YAI is residue-first.
Store is not memory.
Graph is not lineage.
Index/query is not RAG.
Memory is not chat history.
Projection is not UI state.
Reconcile is not recovery execution.
```

SPINE.3R data-plane doctrine:

```text
shared memory = hot state, not truth
journal = replay
hot state = live cache, not truth
LMDB = durable record plane
Ladybug = operational graph persistence / relation truth
RuntimeGraph = active in-memory case graph / working set
DuckDB = derived facts / analytics
HNSW = candidate retrieval index
projection = live versioned cognitive view
```

SPINE.29 record-plane doctrine:

```text
LMDB is durable indexed record lookup
hot state is not LMDB
journal remains replay/audit
schema = yai.record.v1
record:id / record:case / record:kind / record:subject
SPINE.30 LMDB Record Write Path
SPINE.34 LMDB Record Plane Freeze
no journal fallback
```

SPINE.4 observability/evaluation doctrine:

```text
YAI measures whether the live case remains fresh, causal, provenanced,
coherent, complete, replayable and useful enough for controlled behavior.
```

Case View Quality fields:

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

Model/provider experiment doctrine after SPINE.3R:

```text
L0 provider scouting can start immediately outside the core
model/provider implementation follows the NEW.22-NEW.30 data-plane foundation
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
Rust owns hot state mechanics, store, journal, record codec, graph,
index/query, fact derivation, memory, live projection, reconcile, retention and
integrity.
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
do not postpone all yai-dev residue handling to a final cleanup pass
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
../engineering/observability-evaluation-plane.md
../engineering/case-view-quality.md
../engineering/model-provider-experiment-ladder.md
../engineering/naked-model-case-test-plan.md
```
