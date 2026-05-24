# Three-Repo Roadmap

This is the active engineering roadmap for the current YAI core transition.

From SPINE.20 onward, `yai-core` uses one linear progression. Macro names are
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
| `yai-core` | Canonical local AI operational control core. | Completed foundation through SPINE.26 hot state projection freshness integration. | SPINE.27 Hot State CLI + Manual Validation. |
| `yai` | Old/current repo. | Transition concept mine and future `ai-environment` source material. | ENV.CANON.0 later. |
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
bidirectional old-yai extraction and residue normalization mandatory for future
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
SPINE.25 links hot state to case session, case world, case context, active
thread, participant view and decision/receipt-driven projection staleness.
SPINE.26 makes projection freshness consumer-aware: model/agent views require
stricter refresh posture than operator, audit and debug views.

Do not schedule future work with the old NEW numbering. The next active
delivery is SPINE.27.

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
SPINE.25  Hot State Case Session / Context Integration              done
SPINE.26  Hot State Projection Freshness Integration                 done
SPINE.27  Hot State CLI + Manual Validation                         planned
SPINE.28  Hot State Freeze                                          planned

SPINE.29  LMDB Record Plane Doctrine + Schema                       planned
SPINE.30  LMDB Record Write Path                                    planned
SPINE.31  LMDB Record Read / Query Path                             planned
SPINE.32  LMDB Case / Subject / Receipt Indexes                     planned
SPINE.33  LMDB CLI + Manual Validation                              planned
SPINE.34  LMDB Record Plane Freeze                                  planned

SPINE.35  Journal Replay Doctrine + Parser Hardening                planned
SPINE.36  Journal Replay to LMDB                                    planned
SPINE.37  Replay Idempotency + Schema Version Handling              planned
SPINE.38  Replay Diagnostics / Rebuild Report                       planned
SPINE.39  Journal Replay Freeze                                     planned

SPINE.40  Ladybug Graph Doctrine + Schema                           planned
SPINE.41  Ladybug Graph Write Path                                  planned
SPINE.42  Ladybug Graph Query / Causal Path                         planned
SPINE.43  Graph Rebuild from LMDB / Journal                         planned
SPINE.44  Graph Diagnostics / Orphan Edge Checks                    planned
SPINE.45  Graph Plane Freeze                                        planned

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
SPINE.119 AI-Environment Harness Prep                               planned
SPINE.120 Local Core Milestone Freeze                               planned
```

## Implementation Wave Rule

Each SPINE.N must be small enough to implement, validate and manually inspect
in one delivery. A wave may have subtasks, but it must not contain an entire
backend, model program or host-control family. If a proposed wave needs several
independent command surfaces, persistence paths, rebuild paths or smoke suites,
split it before implementation.

Every future wave delivery must include a Command Surface section. If the wave
changes no command surface, it must say so explicitly.

## External Repo Tracks

These tracks remain later and downstream from this `yai-core` roadmap:

| Track | Meaning |
|---|---|
| ENV.CANON | Turn old `yai` into future `ai-environment` concept mine, lab and harness. |
| CONSOLE.CANON | Align operator UX to projections and interfaces. |

The `interfaces` track is no longer listed here. It is governed by the dedicated
INTF + STUDIO spine in the `interfaces` repository:

```text
docs/intf-studio-spine.md
```

## Non-Goals

This roadmap does not rename repos, create `ai-environment`, touch `console`,
implement data planes, or make archived docs active again.

It also does not govern the `interfaces` roadmap anymore. The `interfaces`
repo now owns its own INTF + STUDIO spine at:

```text
interfaces/docs/intf-studio-spine.md
```
