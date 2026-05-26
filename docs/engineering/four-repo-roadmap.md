# Repository Roadmap

This is the active engineering roadmap for the current YAI core transition.

From SPINE.20 onward, `yai` uses one linear progression. Macro names are
labels inside a wave, not a second calendar.

The `interfaces` repo is no longer governed inside this roadmap. Its canonical
roadmap now lives in the `interfaces` repo at:

```text
docs/intf-studio-spine.md
```

That document owns the INTF.SPINE.* roadmap and the downstream STUDIO.SPINE.*
roadmap.

```text
SPINE.N = main delivery number
Macro = affected system areas
Subdeliveries = nested work inside that one delivery
```

## Current Status

| Repo | Role | Status | Next |
|---|---|---|---|
| `yai` | Canonical local AI operational control system. | Completed foundation through SPINE.45 Graph + RuntimeGraph Freeze. | SPINE.46 DuckDB Fact Plane Doctrine + Schema. |
| `yai-dev` | Development lab, concept mine, harness and scenario workspace. | Old/current repo renamed to `yai-dev`; useful material is extracted into `yai` by explicit SPINE waves. | DEV.0 role note, then wave-coupled cleanup. |
| `console` | Operator client / TUI / human UX. | Downstream consumer of projections and interfaces. | CONSOLE.CANON.0 later. |

`interfaces` has been removed from this roadmap because it now owns a dedicated
interface/client spine in the `interfaces` repository:

```text
interfaces/docs/intf-studio-spine.md
```

That document governs both `INTF.SPINE.*` and the downstream `STUDIO.SPINE.*`
client roadmap.

## Completed Foundation

Foundation waves NEW.0 through NEW.19 are complete prehistory for the linear
SPINE.20+ roadmap. They established skeleton guards, persistent journal,
control gate, filesystem carrier, graph, memory candidate, reconcile,
projection, query, Rust engine integration, daemon IPC, daemon-backed loop,
target layout, Rust/C source movement, engine bridge split, case-world records,
live case context, observability doctrine, interaction threads and build/guard
realignment. SPINE.6A compressed engineering docs. SPINE.6B made
bidirectional yai-dev extraction and residue normalization mandatory for future
implementation waves. SPINE.20 established the local `YAI_HOME` runtime layout
and install/uninstall/doctor targets. SPINE.20A rebased the active roadmap so
pack materialization becomes the next canonical spine step. SPINE.21 made
packs first-class case materialization units and defined their format,
lifecycle and materialization boundaries. SPINE.22 freezes the filesystem and
runtime foundation before data-plane implementation begins. SPINE.23 introduces
hot state as the first live data plane: an in-process cache plus
`YAI_HOME/run/hot-state.json` snapshot. SPINE.23A expands the compressed
SPINE.23-SPINE.64 plan into smaller implementable SPINE.23-SPINE.120
deliveries and makes command surface declaration mandatory.
SPINE.24 hardens the hot-state runtime snapshot as schema
`yai.hot_state.v1`, temp-and-rename writes and missing/corrupt status handling.
SPINE.24A retroactively maps SPINE.20-SPINE.24 primitives to canonical command
views, manual command tests and expected output lines.
SPINE.25 links hot state to case session, case world, case context, active
thread, participant view and decision/receipt-driven projection staleness.
SPINE.26 makes projection freshness consumer-aware: model/agent views require
stricter refresh posture than operator, audit and debug views. SPINE.27
stabilizes the manual command surface for `yai hot status`, `yai doctor` and
`yai projection inspect`. SPINE.28 freezes hot state as a non-authoritative live
cache before durable LMDB record-plane work begins. SPINE.28A renames the
canonical core repository to `yai` and the old concept-mine repository to
`yai-dev`. SPINE.28B removes local virtualenv roots, archives README-only
ingest placeholders and marks transitional C data shims as temporary engine
drain surfaces. SPINE.29 defines LMDB as durable indexed record lookup under
`YAI_HOME/store/lmdb`, adds `yai store status` and keeps hot state, journal,
graph, facts and memory separate before the write path begins. SPINE.30 adds
the Rust LMDB write path, minimal `records_by_id`, `records_by_case` and
`records_by_kind` indexes, `yai store summary` and daemon-loop mirroring from
journal to LMDB. SPINE.31 adds `yai store record get` and
`yai store record list` over the id, case and kind indexes without adding
journal fallback reads. SPINE.32 adds derived subject and receipt indexes plus
CLI list filters for both. SPINE.33 freezes LMDB CLI/manual validation.
SPINE.33A through SPINE.33I harden the carrier/control/receipt/divergence
substrate. SPINE.33J through SPINE.33M reserve retrieval, model runtime,
Context Compiler and RuntimeGraph boundaries. SPINE.34 freezes the LMDB record
plane as `yai.record.v1` durable indexed lookup before journal replay begins.
SPINE.35 hardens the journal replay boundary with diagnostic parser inspection
and no LMDB writes. SPINE.36 adds explicit journal replay to LMDB with dry-run
reporting and basic idempotent replay. SPINE.37 hardens replay idempotency with
journal identity, schema compatibility, replay cursor metadata and
`yai journal replay-status`. SPINE.38 adds `yai.replay_report.v1` durable replay
diagnostics with `journal_identity`, `compatibility`, `cursor_line`,
`records_written`, `records_duplicate`, `invalid_entries` and failed report
evidence. SPINE.39 freezes journal replay as the controlled materialization
path from append-only replay/audit residue into LMDB before graph persistence
begins. SPINE.40 defines Graph persistence as durable typed relations and
RuntimeGraph as the in-memory active case working set; HNSW finds candidate
nodes, HNSW is not graph truth, Context Compiler renders controlled views and
Projection does not disappear. Persistent truth on disk. Computational shape in memory.
SPINE.41 adds the first minimal graph relation write path:
`yai.graph_relation.v1` relations are derived from LMDB records, written to
`lmdb_graph_relations_v0` and inspected by case with
`yai graph materialize --case <case_ref>` and
`yai graph relations --case <case_ref>`. RuntimeGraph remains planned.
Ladybug integration remains future. SPINE.42 adds an active-minimal
RuntimeGraph in-memory working set loaded from graph relations with
`yai graph runtime-load --case <case_ref>`. The working set is
per-command ephemeral; resident service, HNSW and Context Compiler remain
future.
SPINE.43 adds RuntimeGraph rebuild: `yai graph rebuild --case <case_ref>
--from journal --path <journal.jsonl>` runs journal replay to LMDB, graph
relations materialization and RuntimeGraph load, while `yai graph rebuild
--case <case_ref> --from graph-relations` rebuilds from existing graph
relations. Rebuild emits `yai.runtime_graph_rebuild_report.v1`,
`runtime_graph_rebuild` and `runtime-summary` evidence. RuntimeGraph is not
durable truth. RuntimeGraph is not durable truth and resident service planned
remains the posture. The active lab path is `docs/labs/filesystem-loop`.
SPINE.44A adds an active operator review loop. SPINE.44B adds the CLI review
interaction surface: `control pending`, `control show`, `control review --interactive`, `control watch`, `control wait`, `next_commands` and
`not_a_tty` behavior. `pending_operator` remains visible.
`subject:linenoise-terminal is prompt surface` and
`subject:operator-reviewer is review authority`; approve may execute the safe reviewed write, while deny,
defer and quarantine keep `carrier_attempted: false` and
`execution_performed: false`. The active lab path is
`docs/labs/filesystem-loop`.

SPINE.44C aligns the review matrix across `docs/labs/filesystem-loop` and
`docs/labs/filesystem-loop`: approve, deny, defer, quarantine,
`pending_operator`, `next_commands`, `wait timeout`, bounded `watch`,
`carrier_attempted: false` and `execution_performed: false` are tested. Model
proposal observed, model cannot approve, and automatic proposed-op gate import
is future work.

Review matrix guard: model proposal observed; model cannot approve; automatic
proposed-op gate import is future work.

SPINE.45 freezes graph schema, `yai.graph_relation.v1`, graph materialize,
graph relations, RuntimeGraph runtime-load/runtime-summary, runtime graph
rebuild, `yai.runtime_graph_rebuild_report.v1`, fanout, fanin, neighborhood,
causal path, bounded traversal, edge-kind filter, path found, path not_found
and empty case. RuntimeGraph is not durable truth; plain output remains
parseable and color-aware graph inspection remains doctrine. HNSW future,
Context Compiler future and Ladybug future persistence integration remain
future. `review_request`, `review_decision` and `control_pending` are
graph-visible when refs exist; approve is visible and deny, defer and
quarantine preserve no-execution posture.

Do not schedule future work with the old NEW numbering. The next active
delivery is SPINE.46.

## Canonical Macro Labels

```text
WORLD
RESIDUE
DATA
VIEW
CONTROL
MEMORY
RETRIEVAL
RECONCILE
HOST
MODEL
OBSERVABILITY
EXTERNAL
```

These labels describe impact. They are not numbered workstreams.

## Operational Wave Contract

Future implementation waves operate on both `yai` and corresponding `yai-dev`
residue when the touched concept already exists there.

Required rule:

```text
A wave is not complete until the corresponding yai-dev residue has been classified.
```

Each relevant SPINE.N must read the old files, extract or rewrite useful
concepts into the new grammar, update inventory, normalize yai-dev residue and
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

## Data Context Runtime / RuntimeGraph Doctrine

YAI separates durable truth from runtime computation.

```text
Truth lives on durable planes.
Computation happens in runtime working sets.
```

The canonical data/context split is:

```text
LMDB stores records.
Ladybug stores relations.
DuckDB stores facts.
RuntimeGraph computes over the active case.
HNSW finds candidate nodes.
Context Compiler renders controlled views.
```

Ladybug remains graph persistence, relation truth and rebuild contract.
RuntimeGraph is the in-memory active case graph / working set loaded from
durable truth. HNSW is a rebuildable proximity index over retrieval units bound
to RuntimeGraph nodes. Context Compiler builds temporary controlled projections
and model contexts from RuntimeGraph, retrieval candidates, policy, facts and
token budgets.

The retrieval roadmap therefore includes:

```text
SPINE.58D HNSW Candidate -> RuntimeGraph Expansion
```

This means HNSW finds vectors. RuntimeGraph gives meaning. ContextFrame remains
a temporary compiled artifact, not truth, memory, storage or graph.

## SPINE.20-SPINE.120 Linear Roadmap

SPINE.N is one implementable and manually verifiable delivery. Macro labels stay
inside each wave; they are not parallel schedules. A wave that changes command
surface must list added, changed, removed/deprecated commands, manual command
tests and expected key output lines in its delivery box.

SPINE.23 was completed before this expansion. It delivered more than the new
SPINE.23 title alone implies, so its implementation is recorded as:

```text
SPINE.23  Hot State Doctrine + ABI                                  done
partial coverage: SPINE.24 Hot State Runtime Snapshot
partial coverage: SPINE.25 Hot State Case Session / Context Integration
partial coverage: SPINE.26 Hot State Projection Freshness Integration
partial coverage: SPINE.27 Hot State CLI + Manual Validation
```

SPINE.24 through SPINE.26 reconcile the partial hot-state behavior already
landed in SPINE.23 and make it explicit, validated and manually inspectable.

```text
SPINE.20  Local Runtime Layout                                      done
SPINE.20A Pack Roadmap Rebase                                       done
SPINE.21  Pack Materialization Doctrine                             done
SPINE.22  Filesystem & Runtime Layout Freeze                        done
SPINE.23  Hot State Doctrine + ABI                                  done
SPINE.23A Roadmap Expansion + Command-Test Contract                 done
SPINE.24  Hot State Runtime Snapshot                                done
SPINE.24A Retroactive Command Surface Recovery                      done
SPINE.25  Hot State Case Session / Context Integration              done
SPINE.26  Hot State Projection Freshness Integration                 done
SPINE.27  Hot State CLI + Manual Validation                         done
SPINE.28  Hot State Freeze                                          done
SPINE.28A Repository Identity Cutover                                done
SPINE.28B Internal Source Surface Cleanup                            done

SPINE.29  LMDB Record Plane Doctrine + Schema                       done
SPINE.30  LMDB Record Write Path                                    done
SPINE.31  LMDB Record Read / Query Path                             done
SPINE.32  LMDB Case / Subject / Receipt Indexes                     done
SPINE.33  LMDB CLI + Manual Validation                              done
SPINE.33A Control / Carrier Substrate Primitives                    done
SPINE.33B Operation Dispatch + Multiplex v0                         done
SPINE.33C Carrier Contract v1 + Filesystem Adapter                  done
SPINE.33D Process Carrier v0 / Signal Control                      done
SPINE.33E Host Observation Probe v0 / Bypass Check                 done
SPINE.33F Carrier Coverage Matrix + Mode Taxonomy                  done
SPINE.33G Non-Process Carrier Skeletons                            done
SPINE.33H Carrier Outcome Harness                                  done
SPINE.33I Carrier Receipt / Divergence Hardening                   done
SPINE.33J Retrieval and Model Runtime Roadmap Rebase                done
SPINE.33K Context Compiler / Retrieval / MTP Roadmap Correction     done
SPINE.33L Provider Runtime / LAN Target Surface v0                  done
SPINE.33M Data Context Runtime / RuntimeGraph Doctrine              done
SPINE.34  LMDB Record Plane Freeze                                  done

SPINE.35  Journal Replay Doctrine + Parser Hardening                done
SPINE.36  Journal Replay to LMDB                                    done
SPINE.37  Replay Idempotency + Schema Version Handling              done
SPINE.38  Replay Diagnostics / Rebuild Report                       done
SPINE.39  Journal Replay Freeze                                     done

SPINE.40  Graph Persistence / RuntimeGraph Doctrine + Schema         done
SPINE.41  Graph Relation Write Path                                  done
SPINE.42  RuntimeGraph In-Memory Working Set                         done
SPINE.43  RuntimeGraph Rebuild from Journal / LMDB / Graph Store     done
SPINE.44  RuntimeGraph Query / Causal Path / Diagnostics             done
SPINE.44A Operator Review / Deferred Action Loop v0                  done
SPINE.44B CLI Review Interaction Surface                             done
SPINE.44C Review Loop Test Matrix + Lab Alignment                    done
SPINE.45  Graph + RuntimeGraph Freeze                                done

SPINE.46  DuckDB Fact Plane Doctrine + Schema                       planned
SPINE.47  Receipt / Decision / Projection Facts                     planned
SPINE.48  Model Behavior / Policy Outcome Facts                     planned
SPINE.49  Memory / Divergence / Carrier Facts                       planned
SPINE.50  Fact Reports + CLI Manual Validation                      planned
SPINE.51  Fact Plane Freeze                                         planned

SPINE.52  Live Projection Frame Schema                              planned
SPINE.53  Projection Delta Schema                                   planned
SPINE.54  Projection Freshness / Staleness Engine                   planned
SPINE.55  Projection Authority / Redaction Alignment                planned
SPINE.56  Projection Memory / Divergence Patch                      planned
SPINE.57  Model-Visible Delta Smoke + CLI                           planned
SPINE.58  Live Projection Freeze                                    planned

SPINE.58A Context Compiler / Retrieval Boundary Doctrine            planned
SPINE.58B Retrieval Unit Schema + Embedding Manifest                planned
SPINE.58C HNSW Candidate Index Build / Rebuild                      planned
SPINE.58D HNSW Candidate -> RuntimeGraph Expansion                  planned
SPINE.58E Context Assembly / Rerank / Token Budget Packing          planned
SPINE.58F Retrieval Residue + Cost / Recall Facts                   planned
SPINE.58G Context Compiler / Retrieval Freeze                       planned

SPINE.59  Memory Consolidation Doctrine + Basis Model               planned
SPINE.60  Receipt-Backed Memory Consolidation                       planned
SPINE.61  Graph-Derived Memory Consolidation                        planned
SPINE.62  Policy / Subject Scoped Memory                            planned
SPINE.63  Memory Freshness / Confidence / Contradiction             planned
SPINE.64  Memory Projection Rules + Quality Facts                   planned
SPINE.65  Memory Consolidation Freeze                               planned

SPINE.66  Cross-Plane Reconcile Doctrine                            planned
SPINE.67  Hot vs Record Consistency                                 planned
SPINE.68  Record vs Graph Consistency                               planned
SPINE.69  Graph vs Memory Consistency                               planned
SPINE.70  Projection Stale / Authority Overclaim Detection          planned
SPINE.71  Reconcile Reports + CLI                                   planned
SPINE.72  Reconcile Freeze                                          planned

SPINE.73  Observability Facts Doctrine                              planned
SPINE.74  Trace / Span Records                                      planned
SPINE.75  Timing Samples                                            planned
SPINE.76  Freshness / Case View Quality Facts                       planned
SPINE.77  Model Behavior Facts                                      planned
SPINE.78  Memory Quality Facts                                      planned
SPINE.79  Observability Debug Commands                              planned
SPINE.80  Data Plane Milestone Freeze                               planned

SPINE.81  Ingest Source Material                                    planned
SPINE.82  Claim / Observation / External Receipt Import             planned
SPINE.83  Policy Material Intake                                    planned
SPINE.84  Model Output Intake                                       planned
SPINE.85  Ingest Freeze                                             planned

SPINE.86  Provider Subject Posture                                  planned
SPINE.87  Model Subject Posture                                     planned
SPINE.88  Local Provider Attachment                                 planned
SPINE.89  Remote Provider Attachment                                planned
SPINE.90  Model Output Claim Import                                 planned
SPINE.91  Naked Local Model Case Experiment                         planned
SPINE.92  Model Behavior Trace                                      planned
SPINE.93  Model Carrier                                             planned
SPINE.93A Model Runtime / Runner Boundary Doctrine                  planned
SPINE.93B Model Capability Probe + Decoding Manifest                planned
SPINE.93C Runner Adapter Contract + Invocation Envelope             planned
SPINE.93D MTP / Speculative Decoding Policy + Metrics               planned
SPINE.93E Runner Receipt + KV / Context State Boundary              planned
SPINE.93F Model Runtime Boundary Freeze                             planned
SPINE.94  Model / Provider Freeze                                   planned

SPINE.95  Policy Pack Skeleton                                      planned
SPINE.96  Policy Projection to Model                                planned
SPINE.97  Policy Compliance Experiment Harness                      planned
SPINE.98  Legal / Accounting / Business Domain Packs                planned
SPINE.99  Policy Memory                                             planned
SPINE.100 Unknown Provider Policy Handling                          planned
SPINE.101 Multi-Model Comparative Case Test                         planned
SPINE.102 Human Review / Authority Binding                          planned
SPINE.103 Case Audit Packet                                         planned
SPINE.104 Policy-Control Freeze                                     planned

SPINE.105 Filesystem Carrier Hardening                              planned
SPINE.106 Process Carrier                                           planned
SPINE.107 Host Observation Probes                                   planned
SPINE.108 Network / HTTP Carrier                                    planned
SPINE.109 Database Carrier                                          planned
SPINE.110 Repository / Git Carrier                                  planned
SPINE.111 Carrier Receipts Hardening                                planned
SPINE.112 Recovery / Compensation Posture                           planned
SPINE.113 Daemon Op Execution over IPC                              planned
SPINE.114 Host-Control Freeze                                       planned

SPINE.115 Protocol Fixtures from Core Evidence                      planned
SPINE.116 Interfaces Handoff Prep                                   planned
SPINE.117 Core Conformance Harness                                  planned
SPINE.118 Console Projection Handoff Prep                           planned
SPINE.119 YAI-dev Harness Prep                                      planned
SPINE.120 Local Core Milestone Freeze                               planned
```

## Context Compiler / Retrieval Doctrine

Projection does not disappear. Projection evolves from a static controlled view
into a compiled live context: scoped, redacted, freshness-aware and
consumer-aware.

Context Compiler is the YAI-owned, model-neutral system that assembles a
projection/model context from records, graph persistence, RuntimeGraph, facts,
memory, retrieval candidates, policy and token budget. A compiled model context
is not truth. It is not memory, not the record plane, not the graph and not
policy authority.

Canonical statement:

```text
compiled model context is not truth
```

Canonical chain:

```text
Journal / LMDB / Ladybug / DuckDB
        ↓ rebuild/load
RuntimeGraph
        ↓ retrieval bindings
HNSW
        ↓ candidates
RuntimeGraph expansion + policy/facts filtering
        ↓
Context Compiler
        ↓
Compiled Projection / Model Context
        ↓
Model Runner
```

Use terms carefully:

```text
projection = generic controlled view
model context = projection rendered for a model/provider/runner
ContextFrame = temporary compiled artifact, not durable truth
```

HNSW is candidate retrieval. HNSW is not graph. HNSW is not memory. HNSW is not decision authority.
HNSW is not policy authority. HNSW is a rebuildable
proximity index over retrieval units; it stores vectors and refs, not
authority.

Future query work remains case / subject / policy scoped retrieval and must
return candidates with provenance, not final answers.

Canonical rule:

```text
A YAI decision may use HNSW/vector-retrieval candidates,
but no YAI decision may be justified by vector proximity alone.
```

Operational chain:

```text
LMDB / Ladybug / DuckDB / Memory
        ↓
Retrieval Projection Units
        ↓
Embedding Manifest
        ↓
HNSW Candidate Index
        ↓
Candidate Set
        ↓
Policy + Case Scope + Graph + Facts Filtering
        ↓
Context Compiler
        ↓
Model Context
```

Context assembly uses hard filters before scoring: case scope, subject scope,
authority, redaction, freshness, policy compatibility and operation
compatibility. Scoring is composite: vector similarity, graph relevance,
freshness, behavior facts, risk penalty and redundancy penalty.

## Model Runtime / Runner Doctrine

YAI may own runner boundaries and adapters. YAI should not initially become a
universal inference engine. YAI treats engines/runners as subjects invoked
through model carrier contracts. A runner produces outputs and receipts, not
case authority.

Boundary phrases for future guards: Model Runtime / Runner Boundary; not universal inference engine; runner output is not authority.

Boundary:

```text
Inference engine:
  loads model
  runs tokens
  manages KV/cache/model-specific execution

YAI:
  case binding
  projection
  authority
  prompt/invocation envelope
  model carrier
  provider/runner subject
  receipt
  model output import
  behavior facts
  memory/reconcile consequences
```

MTP belongs to model runtime / runner / decoding strategy. MTP is not retrieval.
MTP is not memory. MTP is not policy authority. MTP is not case truth.

```text
HNSW makes the model read less.
MTP makes the model write faster.
```

YAI tracks MTP as a runner capability and measured behavior: supports_mtp,
mtp_mode, mtp_depth, requires_mtp_weights, requires_draft_model,
backend_support, acceptance_rate, fallback_mode, structured_output_risk,
latency_delta and tokens_per_second_delta. MTP may accelerate generation; it
must not change output authority.

## Provider Runtime / LAN Target Surface Doctrine

The normal future workflow should not require manual provider launch as normal
workflow. Manual provider launch remains a development fallback.

SPINE.33L adds the first inspectable provider runtime surface:

```text
runtime target: local / lan / external
device registry: YAI_HOME/config/devices.jsonl
provider supervisor: planned
provider start dry-run: active
provider logs: YAI_HOME/log/providers
model catalog: not_scanned / planned
model routing: planned
context compiler: planned
retrieval_hnsw: planned
decoding acceleration: planned
```

Rules:

```text
LAN device is not a provider.
LAN device is a runtime target.
Provider runs on a runtime target.
Model belongs to a provider instance.
Case attaches to provider instance.
```

SPINE.33L exposes dry-run plan posture only. It performs no remote start in
SPINE.33L, starts no provider process, performs no LAN discovery, scans no
model files, probes no GPU and invokes no model.

## Implementation Wave Rule

Each SPINE.N must be small enough to implement, validate and manually inspect
in one delivery. A wave may have subtasks, but it must not contain an entire
backend, model program or host-control family. If a proposed wave needs several
independent command surfaces, persistence paths, rebuild paths or smoke suites,
split it before implementation.

Every future wave delivery must include a Command Surface section. If the wave
changes no command surface, it must say so explicitly.

## External Repo Tracks

These tracks remain later and downstream from this `yai` roadmap:

| Track | Meaning |
|---|---|
| DEV | Keep `yai-dev` as lab, workshop, concept mine, provider/scenario harness and release rehearsal space. |
| CONSOLE.CANON | Align operator UX to projections and interfaces. |

The `interfaces` track is no longer listed here. It is governed by the dedicated
INTF + STUDIO spine in the `interfaces` repository:

```text
docs/intf-studio-spine.md
```

## Non-Goals

This roadmap does not touch `interfaces`, `console` or `yai-studio`, implement
data planes, or make archived docs active again.

It also does not govern the `interfaces` roadmap anymore. The `interfaces`
repo now owns its own INTF + STUDIO spine at:

```text
interfaces/docs/intf-studio-spine.md
```

## SPINE.44 RuntimeGraph Query

SPINE.44 RuntimeGraph Query / Causal Path / Diagnostics adds fanout, fanin,
neighborhood and causal path diagnostics as bounded traversal over active-case
RuntimeGraph data. RuntimeGraph is not a generic graph database. The edge-kind
filter is supported for fanout, fanin and neighborhood; max-depth bounds path
diagnostics. Plain output remains parseable and color-aware graph inspection is
future console/studio doctrine under `docs/labs/filesystem-loop`.

## SPINE.44A Operator Review Loop

SPINE.44A adds an active review/deferred/quarantine loop before Graph +
RuntimeGraph Freeze. A filesystem write can enter `require_review`, become
`pending_operator`, and resolve by approve, deny, defer or quarantine.
`subject:linenoise-terminal is prompt surface`; operator reviewer authority is
separate. Deny, defer and quarantine keep `carrier_attempted: false` and
`execution_performed: false` under `docs/labs/filesystem-loop`.
