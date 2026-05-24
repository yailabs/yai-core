# Four-Repo Roadmap

This is the only active engineering roadmap.

From SPINE.20 onward, YAI Core uses one linear progression. Macro names are
labels inside a wave, not a second calendar.

```text
SPINE.N = main delivery number
Macro = affected system areas
Subdeliveries = nested work inside that one delivery
```

## Current Status

| Repo | Role | Status | Next |
|---|---|---|---|
| `yai-core` | Canonical local AI operational control core. | Completed foundation through SPINE.23 hot state plane v0. | SPINE.24 LMDB Record Backend v0. |
| `yai` | Old/current repo. | Transition concept mine and future `ai-environment` source material. | ENV.CANON.0 later. |
| `interfaces` | Projection/API/SDK/conformance repo. | Downstream consumer of `yai-core` truth. | INTF.CANON.0 later. |
| `console` | Operator client / TUI / human UX. | Downstream consumer of projections and interfaces. | CONSOLE.CANON.0 later. |

## Completed Foundation

Foundation waves NEW.0 through NEW.19 are complete prehistory for the linear
SPINE.20+ roadmap. They established skeleton guards, persistent journal,
control gate, filesystem carrier, graph, memory candidate, reconcile,
projection, query, Rust engine integration, daemon IPC, daemon-backed loop,
target layout, Rust/C source movement, engine bridge split, case-world records,
live case context, observability doctrine, interaction threads and build/guard
realignment. SPINE.6A compressed engineering docs. SPINE.6B made
bidirectional old-yai extraction and residue normalization mandatory for future
implementation waves. SPINE.20 established the local `YAI_HOME` runtime layout
and install/uninstall/doctor targets. SPINE.20A rebased the active roadmap so
pack materialization becomes the next canonical spine step. SPINE.21 made
packs first-class case materialization units and defined their format,
lifecycle and materialization boundaries. SPINE.22 freezes the filesystem and
runtime foundation before data-plane implementation begins. SPINE.23 introduces
hot state as the first live data plane: an in-process cache plus
`YAI_HOME/run/hot-state.json` snapshot.

Do not schedule future work with the old NEW numbering. The next active
delivery is SPINE.24.

## Canonical Macro Labels

```text
WORLD
RESIDUE
DATA
VIEW
CONTROL
MEMORY
RECONCILE
HOST
MODEL
OBSERVABILITY
EXTERNAL
```

These labels describe impact. They are not numbered workstreams.

## Operational Wave Contract

Future implementation waves operate on both `yai-core` and corresponding old
`yai` residue when the touched concept already exists there.

Required rule:

```text
A wave is not complete until the corresponding old-yai residue has been classified.
```

Each relevant SPINE.N must read the old files, extract or rewrite useful
concepts into the new grammar, update inventory, normalize old-yai residue and
validate. Details live in `wave-template.md` and
`operational-extraction-contract.md`.

## Pack Materialization Doctrine

Packs are case materialization units.

A pack is not a marketplace product, DLC or prompt bundle. A pack is a way to
materialize methods, procedures, policies, templates, projection rules, output
schemas, memory seeds, actors/tools and validation fixtures into a case.

Pack kinds:

```text
native_yai
user
organization
vertical
pilot
reference
experimental
compat
```

SPINE.21 defines the pack format, lifecycle and materialization rules.

## SPINE.20-SPINE.64 Linear Roadmap

```text
SPINE.20  Local Runtime Layout                                  done
SPINE.20A Pack Roadmap Rebase                                   done
SPINE.21  Pack Materialization Doctrine                         done
SPINE.22  Filesystem & Runtime Layout Freeze                    done
SPINE.23  Hot State / Shared Memory Plane v0                    done
SPINE.24  LMDB Record Backend v0                                planned
SPINE.25  Journal Replay to LMDB v0                             planned
SPINE.26  Ladybug Graph Backend v0                              planned
SPINE.27  DuckDB Fact Backend v0                                planned
SPINE.28  Projection Delta / Live View v0                       planned
SPINE.29  Memory Consolidation over Record + Graph + Facts v0   planned
SPINE.30  Reconcile over Hot / Store / Graph Consistency v0     planned
SPINE.31  Observability / Evaluation Facts v0                   planned
SPINE.32  Data Plane Milestone Freeze                           planned
SPINE.33  Ingest Material Model v0                              planned
SPINE.34  Model / Provider Subject Posture v0                   planned
SPINE.35  Model Output Claim Import v0                          planned
SPINE.36  Policy Pack Skeleton + Model Projection v0            planned
SPINE.37  Naked Local Model Case Experiment v0                  planned
SPINE.38  Model Behavior Trace v0                               planned
SPINE.39  Model Carrier v0                                      planned
SPINE.40  Agent Trace / Tool Call Import v0                     planned
SPINE.41  Policy Compliance Experiment Harness v0               planned
SPINE.42  Legal / Accounting / Business Domain Packs v0         planned
SPINE.43  Policy Memory v0                                      planned
SPINE.44  Unknown Provider Policy Handling v0                   planned
SPINE.45  Multi-Model Comparative Case Test v0                  planned
SPINE.46  Human Review / Authority Binding v0                   planned
SPINE.47  Case Audit Packet v0                                  planned
SPINE.48  Policy-Control Milestone Freeze                       planned
SPINE.49  Filesystem Carrier Hardening                          planned
SPINE.50  Process Carrier v0 with Host Safeguards               planned
SPINE.51  Host Observation Probes v0                            planned
SPINE.52  Network / HTTP Carrier v0                             planned
SPINE.53  Database Carrier v0                                   planned
SPINE.54  Repository / Git Carrier v0                           planned
SPINE.55  Carrier Receipts Hardening                            planned
SPINE.56  Recovery / Compensation Posture v0                    planned
SPINE.57  Daemon Op Execution over IPC                          planned
SPINE.58  Host-Control Milestone Freeze                         planned
SPINE.59  Protocol Fixtures from Core Journals                  planned
SPINE.60  Interfaces Handoff Prep                               planned
SPINE.61  Core Conformance Harness                              planned
SPINE.62  Console Projection Handoff Prep                       planned
SPINE.63  AI-Environment Harness Prep                           planned
SPINE.64  Local Core Milestone Freeze                           planned
```

### SPINE.20 - Local Runtime Layout

Status: done.

Macro: HOST / DATA / WORLD / OBSERVABILITY

Purpose: stop using `build/tmp` as the normal operating environment.

Subdeliveries: define `YAI_HOME`; install local binaries `yai` and `yaid`;
define run/store/log/tmp dirs; default socket path; local doctor output; local
cleanup/uninstall; manual daemon start/stop/status; validation with installed
`yai`/`yaid`; docs and freeze.

Old-yai audit focus: local runtime, launcher, daemon, environment and install
scripts.

Quality gate: installed `yai` and `yaid` run the existing smoke path without
depending on transient build paths.

### SPINE.20A - Pack Roadmap Rebase

Status: done.

Macro: WORLD / DATA / VIEW / CONTROL / MEMORY / EXTERNAL

Purpose: update the active roadmap before SPINE.21 so pack materialization
becomes a canonical spine step.

Subdeliveries: insert SPINE.21 Pack Materialization Doctrine; shift the
post-SPINE.20 sequence consistently; add pack materialization preview; update
status, data-plane ordering, wave template and extraction plan surfaces; no
implementation.

Old-yai audit focus: read-only status/audit only if needed.

Quality gate: active docs use one SPINE.20+ linear numbering sequence and no
active doc says SPINE.21 is filesystem freeze.

### SPINE.21 - Pack Materialization Doctrine

Status: done.

Macro: WORLD / RESIDUE / VIEW / CONTROL / MEMORY / MODEL / EXTERNAL

Purpose: define packs as first-class case materialization units.

Must define: pack kinds; pack lifecycle; pack manifest shape; pack folder
shape; pack materialization into `case_domain` / `case_attachment` /
`case_binding`; policy material; projection rules; authority scopes;
procedures; templates; output schemas; memory seeds; fixtures; limits /
budgets; native/user/organization/vertical pack distinction.

Non-goals: no marketplace; no billing; no pack installer; no pack runtime; no
agent pack execution.

Old-yai audit focus: `../yai/src/case/packs/*`,
`../yai/src/case/materialization/*`, `../yai/src/case/policy/*`,
`../yai/src/case/subjects/*`, `../yai/src/case/scope/*`,
`../yai/src/capabilities/*`, `../yai/src/agents/roles/*`,
`../yai/src/agents/composition/*`, `../yai/src/models/frame/*`,
`../yai/src/substrate/sources/*`, `../yai/src/substrate/views/*`.

Quality gate: pack doctrine defines materialization boundaries without creating
pack runtime, installer, marketplace or pack directories.

### SPINE.22 - Filesystem & Runtime Layout Freeze

Status: done.

Macro: HOST / DATA / WORLD / OBSERVABILITY / EXTERNAL

Purpose: freeze the current filesystem, runtime layout, documentation canon
and checks before starting real data-plane implementation.

Subdeliveries: verify source layout; verify runtime layout; verify active docs
compactness; verify roadmap status; verify pack doctrine guard; verify
operational extraction contract guard; verify install-local/uninstall-local;
verify installed `yai`/`yaid` daemon loop; verify old roots absent; verify old
`yai` dirty state is not touched; update stale status surfaces; full
validation; milestone freeze.

Old-yai audit focus: none unless stale path language overlaps active docs.

Quality gate: source layout, runtime layout, active docs, pack doctrine,
operational extraction contract, install/uninstall and installed daemon loop
all validate.

### SPINE.23 - Hot State / Shared Memory Plane v0

Macro: DATA / WORLD / VIEW / OBSERVABILITY

Purpose: make the case live without rereading journal as the primary path.

Subdeliveries: hot-state doctrine; shared-memory contract; active case/session
state; active projection frame state; pending ops and obligations; carrier
locks; latest receipt and delta; freshness flags; daemon integration; hot-state
lifecycle smoke; docs and freeze.

Old-yai audit focus: `src/runtime/machine/*`, `src/runtime/sessions/*`,
`src/runtime/lifecycle/*`, `src/runtime/operator/operator_context.c`,
`src/runtime/operator/queue_gate.c`, `src/runtime/operator/queue_view.c`,
`src/case/continuity/*`, `src/case/materialization/*`,
`src/substrate/working/*`, `src/substrate/views/*`, `src/substrate/store/*`.

Quality gate: daemon writes a hot-state snapshot, `yai doctor` and
`yai hot status` expose readiness/freshness, and durable truth remains
residue-backed.

### SPINE.24 - LMDB Record Backend v0

Macro: DATA / RESIDUE / WORLD

Purpose: introduce the durable record lookup plane.

Subdeliveries: LMDB backend doctrine; record key schema; case/subject/receipt
and decision namespaces; write path from residue; read path by id/kind/case and
subject; Rust engine integration; C bridge; rebuild safety; record lookup
smoke; docs and freeze.

Old-yai audit focus: `src/substrate/store/*`, `src/substrate/records/*`.

Quality gate: journal is no longer the only way to read case records.

### SPINE.25 - Journal Replay to LMDB v0

Macro: DATA / RESIDUE / OBSERVABILITY

Purpose: make journal replay/audit, not primary operating state.

Subdeliveries: replay doctrine; journal parser hardening; replay to LMDB;
idempotency rules; schema version handling; replay diagnostics; corrupt record
handling; rebuild report; journal-to-LMDB rebuild smoke; docs and freeze.

Old-yai audit focus: old replay, drain and journal persistence paths.

Quality gate: a case can be rebuilt from journal into the record plane with a
diagnostic report.

### SPINE.26 - Ladybug Graph Backend v0

Macro: DATA / RECONCILE / MEMORY

Purpose: make graph a persistent causal plane, not only graph-edge records in
the journal.

Subdeliveries: graph backend doctrine; Ladybug storage boundary; edge schema;
node/ref schema; write graph edges from records; causal path query; rebuild
graph from LMDB/journal; orphan edge diagnostics; receipt-to-decision-to-subject
reconstruction; docs and freeze.

Old-yai audit focus: `src/substrate/graph/*`, `src/lineage/*`.

Quality gate: receipt chains reconstruct from the graph backend.

### SPINE.27 - DuckDB Fact Backend v0

Macro: DATA / OBSERVABILITY / MODEL / CONTROL

Purpose: start the analytical plane for research, debug and product reporting.

Subdeliveries: fact-plane doctrine; DuckDB schema; receipt facts; decision
facts; projection facts; memory facts; model behavior facts; policy outcome
facts; basic reports; facts extraction smoke; docs and freeze.

Old-yai audit focus: `src/analytics/*`, `src/analytics/features/*`.

Quality gate: facts derive from residue without becoming operational truth.

### SPINE.28 - Projection Delta / Live View v0

Macro: VIEW / DATA / MODEL / OBSERVABILITY

Purpose: make projection a live versioned view instead of a static summary.

Subdeliveries: live projection doctrine; projection frame schema; projection
delta schema; previous/next frame links; stale/fresh rules; hot-state
integration; memory patch integration; divergence exposure; authority
alignment; model-visible delta smoke; docs and freeze.

Old-yai audit focus: `src/substrate/views/*`, `src/case/surface/*`,
`src/models/frame/*`.

Quality gate: model-visible context can be refreshed by delta with provenance
and freshness.

### SPINE.29 - Memory Consolidation over Record + Graph + Facts v0

Macro: MEMORY / DATA / VIEW / OBSERVABILITY

Purpose: move from memory candidates to consolidated operational memory.

Subdeliveries: memory consolidation doctrine; memory basis model;
receipt-backed consolidation; graph-derived consolidation; policy-aware memory;
subject-scoped memory; contradiction check; memory freshness/confidence; memory
projection rules; memory quality facts; consolidation smoke; docs and freeze.

Old-yai audit focus: `src/substrate/memory/*`, `src/lineage/*`,
`src/agents/grounding/memory_strategy.c`.

Quality gate: memory has explicit basis, scope, freshness and projection
permissions.

### SPINE.30 - Reconcile over Hot / Store / Graph Consistency v0

Macro: RECONCILE / DATA / OBSERVABILITY

Purpose: verify the system across data planes.

Subdeliveries: hot vs record consistency; record vs graph consistency; graph vs
memory consistency; projection stale detection; authority overclaim detection;
memory without basis detection; missing receipt detection; recovery posture;
reconcile reports; docs and freeze.

Old-yai audit focus: `src/substrate/signals/*`, `src/runtime/observation/*`.

Quality gate: mismatches become explicit divergence, not silent repair.

### SPINE.31 - Observability / Evaluation Facts v0

Macro: OBSERVABILITY / DATA / MODEL / RECONCILE

Purpose: make quality and evaluation facts queryable before the data-plane
milestone freeze.

Subdeliveries: observability fact doctrine; case-view quality facts; freshness
facts; provenance facts; replayability facts; model behavior evaluation facts;
memory quality facts; divergence exposure facts; report smoke; docs and freeze.

Old-yai audit focus: `src/analytics/*`, `src/runtime/observation/*`,
`src/substrate/signals/*`.

Quality gate: evaluation facts derive from residue without becoming
operational truth.

### SPINE.32 - Data Plane Milestone Freeze

Macro: DATA / OBSERVABILITY

Purpose: close the first real data-plane foundation.

Subdeliveries: rebuild full case from journal; rebuild LMDB; rebuild graph;
rebuild facts; rebuild memory; validate projection freshness; validate
reconcile; full smoke suite; documentation freeze; release checkpoint.

Old-yai audit focus: only gaps discovered by rebuild and smoke evidence.

Quality gate: full case rebuild and projection freshness validation pass.

### SPINE.33 - Ingest Material Model v0

Macro: RESIDUE / WORLD / DATA

Purpose: make everything entering the case explicit and provenanced.

Subdeliveries: ingest doctrine; source material record; claim record;
observation record; external receipt import; policy material intake; model
output intake; agent trace intake placeholder; provenance requirements; docs
and freeze.

Old-yai audit focus: `src/case/materialization/*`, `src/substrate/records/*`.

Quality gate: no external material enters without source, provenance and case
binding.

### SPINE.34 - Model / Provider Subject Posture v0

Macro: MODEL / WORLD / VIEW / CONTROL

Purpose: formalize model and provider as case subjects.

Subdeliveries: provider attachment model; local provider posture; remote
provider posture; unknown internal policy flag; model subject posture; locality,
visibility and runtime fields; provider-declared policy refs; default projection
posture; attach provider/model smoke; docs and freeze.

Old-yai audit focus: `src/models/*`, `src/runtime/provider/*`,
`src/case/policy/*`.

Quality gate: provider/model attachment does not grant policy or execution
authority by itself.

### SPINE.35 - Model Output Claim Import v0

Macro: MODEL / RESIDUE / RECONCILE

Purpose: treat model output as case material, not truth.

Subdeliveries: model output record; `model_interpretation` hardening; claim
extraction; proposed attempt detection; unsupported claim marking; authority
overclaim marking; provenance to projection frame; store/query integration;
model output import smoke; docs and freeze.

Old-yai audit focus: `src/models/*`, `src/agents/grounding/*`.

Quality gate: unsupported claims and overclaims are visible as residue.

### SPINE.36 - Policy Pack Skeleton + Model Projection v0

Macro: CONTROL / VIEW / MODEL

Purpose: project materialized policy to models instead of casual prompt text.

Subdeliveries: policy pack schema; policy source material; policy claim/rule
skeleton; `projection_rule` from policy; `authority_scope` from policy; model
projection template; review requirement projection; forbidden/allowed action
projection; policy-pack-to-projection smoke; docs and freeze.

Old-yai audit focus: `src/case/policy/*`, `src/substrate/views/*`.

Quality gate: policy appears both cognitively through projection and
operationally through gates.

### SPINE.37 - Naked Local Model Case Experiment v0

Macro: MODEL / VIEW / CONTROL / MEMORY / OBSERVABILITY

Purpose: run the first real naked model inside a case.

Subdeliveries: local model runtime profile; model subject attach; projection to
model; model output import; claim/proposed op control; receipt/memory
generation; behavior fact capture; repeated run fixture; naked model case
smoke; docs and freeze.

Old-yai audit focus: `src/models/*`, `src/runtime/provider/*`,
`src/agents/grounding/*`.

Quality gate: a model can participate without agent-framework ownership.

### SPINE.38 - Model Behavior Trace v0

Macro: MODEL / OBSERVABILITY / RECONCILE

Purpose: make model behavior measurable.

Subdeliveries: behavior taxonomy; refusal fact; unsafe proposal fact;
unsupported claim fact; authority overclaim fact; review request fact; memory
use fact; projection variant comparison; behavior trace smoke; docs and freeze.

Old-yai audit focus: `src/analytics/signals/*`, `src/runtime/observation/*`.

Quality gate: refusal, unsupported claim and overclaim evidence can be queried.

### SPINE.39 - Model Carrier v0

Macro: HOST / MODEL / CONTROL

Purpose: invoke a model as a controlled effect.

Subdeliveries: model carrier doctrine; local model invocation carrier; provider
invocation receipt; `model.invoke` op attempt; decision before model call;
output receipt; timeout/error handling; no raw-data policy enforcement; model
carrier smoke; docs and freeze.

Old-yai audit focus: `src/runtime/provider/*`, `src/runtime/execution/*`.

Quality gate: every model call has decision and receipt posture.

### SPINE.40 - Agent Trace / Tool Call Import v0

Macro: MODEL / EXTERNAL / RESIDUE / CONTROL

Purpose: import agents as external subjects, not core owners.

Subdeliveries: agent trace doctrine; external harness subject; tool call
import; MCP call import; planner output import; agent claim/attempt mapping;
trace provenance; control over tool attempts; agent trace import smoke; docs
and freeze.

Old-yai audit focus: `src/agents/grounding/*`, `src/capabilities/mcp/*`,
`src/capabilities/external/*`.

Quality gate: tool attempts imported from agents still pass through control.

### SPINE.41 - Policy Compliance Experiment Harness v0

Macro: CONTROL / MODEL / OBSERVABILITY

Purpose: turn model/policy experiments into measurable evidence.

Subdeliveries: experiment doctrine; policy/projection variants; model
comparison matrix; scenario fixture format; compliance scoring;
refusal/overclaim scoring; memory feedback scoring; report generation; policy
experiment smoke; docs and freeze.

Old-yai audit focus: `src/analytics/*`, `src/case/policy/*`.

Quality gate: experiment reports derive from facts and residue.

### SPINE.42 - Legal / Accounting / Business Domain Packs v0

Macro: CONTROL / MODEL / VIEW / MEMORY

Purpose: move YAI toward non-dev operational cases.

Subdeliveries: domain pack doctrine; legal review pack; accounting practice
pack; business admin pack; domain subject templates; domain policy templates;
projection templates; review obligations; domain pack smoke; docs and freeze.

Old-yai audit focus: old scenario/domain examples only.

Quality gate: domain packs remain case material, not hardcoded core behavior.

### SPINE.43 - Policy Memory v0

Macro: CONTROL / MEMORY / VIEW

Purpose: reuse policy experience without making policy magical.

Subdeliveries: policy memory doctrine; decision history memory; review outcome
memory; policy conflict memory; provider policy conflict memory; projection
policy memory; reuse in future gates; policy memory smoke; docs and freeze.

Old-yai audit focus: `src/case/policy/*`, `src/substrate/memory/*`.

Quality gate: policy memory has decision and receipt basis.

### SPINE.44 - Unknown Provider Policy Handling v0

Macro: MODEL / CONTROL / RECONCILE

Purpose: treat closed providers as black boxes with unknown policy.

Subdeliveries: unknown policy doctrine; declared provider policy refs; observed
behavior profile; provider refusal classification; provider conflict
divergence; remote default projection posture; local/open model posture;
unknown provider handling smoke; docs and freeze.

Old-yai audit focus: `src/runtime/provider/*`, `src/models/*`.

Quality gate: provider policy conflicts become divergence or posture, not
assumed truth.

### SPINE.45 - Multi-Model Comparative Case Test v0

Macro: MODEL / OBSERVABILITY / DATA

Purpose: compare models scientifically inside the same case.

Subdeliveries: comparison doctrine; same case multi-model setup; same
projection variant; behavior fact comparison; latency/cost comparison; policy
compliance comparison; memory usefulness comparison; report; docs and freeze.

Old-yai audit focus: model and analytics fixtures.

Quality gate: comparison reports use shared case residue and fact data.

### SPINE.46 - Human Review / Authority Binding v0

Macro: CONTROL / WORLD / VIEW

Purpose: make human operators explicit authorities.

Subdeliveries: human authority subject; reviewer binding; review request
record; approval/deny record; review receipt; obligation closure; review
projection; human review smoke; docs and freeze.

Old-yai audit focus: operator/review runtime material.

Quality gate: review authority is bound to case world and receipts.

### SPINE.47 - Case Audit Packet v0

Macro: VIEW / OBSERVABILITY / EXTERNAL

Purpose: produce readable and reusable evidence.

Subdeliveries: audit packet doctrine; case summary; receipt bundle; decision
basis bundle; graph path bundle; memory basis bundle; divergence bundle;
redaction/audit posture; audit packet smoke; docs and freeze.

Old-yai audit focus: report/export material.

Quality gate: audit packets preserve redaction, provenance and quality posture.

### SPINE.48 - Policy-Control Milestone Freeze

Macro: CONTROL / MODEL / VIEW / MEMORY

Purpose: close the policy/model block.

Subdeliveries: policy pack validation; model projection validation; authority
validation; review validation; policy memory validation; model behavior
reports; docs freeze; full smoke.

Old-yai audit focus: only unresolved policy/model residues.

Quality gate: policy/model/review behavior is reproducible from residue.

### SPINE.49 - Filesystem Carrier Hardening

Macro: HOST

Purpose: harden the filesystem carrier.

Subdeliveries: path canonicalization; sandbox boundary proof; permission
posture; symlink/path traversal tests; before/after hash hardening; receipt
details; adversarial filesystem tests; docs and freeze.

Old-yai audit focus: filesystem carrier and sandbox material.

Quality gate: adversarial filesystem tests pass.

### SPINE.50 - Process Carrier v0 with Host Safeguards

Macro: HOST / CONTROL / OBSERVABILITY

Purpose: add controlled process execution.

Subdeliveries: process carrier doctrine; command subject model; environment
boundary; stdout/stderr receipt; exit status policy; timeout/kill; resource
limits; process smoke/adversarial; docs and freeze.

Old-yai audit focus: process execution and runtime carrier material.

Quality gate: process effects produce bounded receipts and timeout posture.

### SPINE.51 - Host Observation Probes v0

Macro: HOST / RECONCILE / OBSERVABILITY

Purpose: observe host state and expose bypass.

Subdeliveries: host observation doctrine; filesystem probe; process probe;
network probe placeholder; observation receipt; external bypass detection; host
observation smoke; docs and freeze.

Old-yai audit focus: runtime observation probes.

Quality gate: observed bypass creates residue and reconcile posture.

### SPINE.52 - Network / HTTP Carrier v0

Macro: HOST / CONTROL / MODEL

Purpose: add controlled network effects.

Subdeliveries: network carrier doctrine; endpoint subject; request policy;
response receipt; timeout/error receipt; redaction/no-raw-data gate; network
carrier smoke; docs and freeze.

Old-yai audit focus: external capability and HTTP adapter material.

Quality gate: network requests are gated and receipted.

### SPINE.53 - Database Carrier v0

Macro: HOST / DATA / CONTROL

Purpose: add controlled database effects.

Subdeliveries: database carrier doctrine; database subject; read/query receipt;
write/mutation gate; transaction receipt; rollback posture; database carrier
smoke; docs and freeze.

Old-yai audit focus: database/tool adapter material.

Quality gate: mutation paths require control and receipt posture.

### SPINE.54 - Repository / Git Carrier v0

Macro: HOST / RESIDUE / OBSERVABILITY

Purpose: add controlled repository effects.

Subdeliveries: repository carrier doctrine; repo subject; diff receipt; patch
attempt; commit boundary; branch/status observation; repo carrier smoke; docs
and freeze.

Old-yai audit focus: repository and git operation material.

Quality gate: repo effects preserve diff, status and receipt evidence.

### SPINE.55 - Carrier Receipts Hardening

Macro: HOST / RESIDUE / RECONCILE

Purpose: harden carrier receipt semantics.

Subdeliveries: carrier receipt schema; status taxonomy; before/after state;
error envelope; external receipt import; receipt validation; receipt replay;
docs and freeze.

Old-yai audit focus: carrier receipt and external receipt material.

Quality gate: receipt replay validates carrier outcome posture.

### SPINE.56 - Recovery / Compensation Posture v0

Macro: RECONCILE / HOST / CONTROL

Purpose: represent recovery without silent repair.

Subdeliveries: recovery doctrine; compensation posture; no silent repair rule;
recovery obligation; recovery projection; compensation receipt; recovery
posture smoke; docs and freeze.

Old-yai audit focus: recovery and compensation material.

Quality gate: recovery is explicit posture and receipt, not hidden mutation.

### SPINE.57 - Daemon Op Execution over IPC

Macro: HOST / WORLD / CONTROL

Purpose: execute controlled operations through daemon IPC.

Subdeliveries: op request envelope; `case_session` lookup; authority check;
control decision; carrier dispatch; receipt response; projection update; IPC op
execution smoke; docs and freeze.

Old-yai audit focus: daemon, runtime session and IPC execution material.

Quality gate: IPC operation execution returns decision and receipt evidence.

### SPINE.58 - Host-Control Milestone Freeze

Macro: HOST / CONTROL / OBSERVABILITY

Purpose: close host-control foundation.

Subdeliveries: filesystem carrier validation; process carrier validation;
network/database/repo validation; carrier receipt validation; reconcile
validation; host observability validation; docs freeze.

Old-yai audit focus: only unresolved host-control residues.

Quality gate: host-control smoke and adversarial coverage pass.

### SPINE.59 - Protocol Fixtures from Core Journals

Macro: EXTERNAL / DATA

Purpose: prepare downstream protocol fixtures from core evidence.

Subdeliveries: fixture doctrine; journal fixture extraction; record fixture
extraction; projection fixture extraction; receipt fixture extraction; schema
candidates; docs and freeze.

Old-yai audit focus: protocol and fixture material.

Quality gate: fixtures derive from real smoke journals and records.

### SPINE.60 - Interfaces Handoff Prep

Macro: EXTERNAL

Purpose: prepare `interfaces` to consume core truth.

Subdeliveries: primitive schema list; operation mapping; projection contracts;
transport needs; SDK boundary; conformance plan; docs and freeze.

Old-yai audit focus: old interface/protocol leakage only.

Quality gate: interfaces remain downstream of projections and protocols.

### SPINE.61 - Core Conformance Harness

Macro: EXTERNAL / OBSERVABILITY

Purpose: define external conformance over core behavior.

Subdeliveries: conformance doctrine; invariant tests; replay tests; projection
tests; carrier tests; behavior fixtures; report format; docs and freeze.

Old-yai audit focus: old tests useful as scenarios only.

Quality gate: conformance can validate core invariants from fixtures.

### SPINE.62 - Console Projection Handoff Prep

Macro: EXTERNAL / VIEW

Purpose: prepare console as projection-first UX.

Subdeliveries: console role reaffirmation; projection-first command map; review
queue projection; receipt view contract; graph view contract; memory view
contract; docs and freeze.

Old-yai audit focus: operator UI concepts only.

Quality gate: console does not own core truth.

### SPINE.63 - AI-Environment Harness Prep

Macro: EXTERNAL / MODEL / HOST

Purpose: prepare the external environment harness.

Subdeliveries: ai-environment role; harness subject model; agent trace adapter
plan; provider experiment fixtures; policy compliance scenarios; release
rehearsal boundary; docs and freeze.

Old-yai audit focus: agents, providers, scenarios and release rehearsal
material.

Quality gate: ai-environment remains outside yai-core ownership.

### SPINE.64 - Local Core Milestone Freeze

Macro: ALL

Purpose: close the local core milestone.

Subdeliveries: full build/check/smoke; case_world validation; data-plane
validation; projection/memory/reconcile validation; model/policy validation;
host carrier validation; observability validation; external handoff validation;
docs freeze; milestone tag.

Old-yai audit focus: final unresolved transition residues only.

Quality gate: full milestone validation passes and status docs freeze.

## External Repo Tracks

These tracks remain later and downstream:

| Track | Meaning |
|---|---|
| ENV.CANON | Turn old `yai` into future `ai-environment` concept mine, lab and harness. |
| INTF.CANON | Align interfaces, schemas, transports, SDK and conformance to `yai-core` truth. |
| CONSOLE.CANON | Align operator UX to projections and interfaces. |

## Non-Goals

This roadmap does not rename repos, create `ai-environment`, touch
`interfaces`, touch `console`, implement data planes, or make archived docs
active again.
