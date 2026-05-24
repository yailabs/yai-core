# Data Plane Roadmap

This is the active data-plane roadmap. It replaces older active-looking data
spine and operational data-plane notes, which are archived under
`docs/archive/engineering/roadmaps/` and `docs/archive/engineering/superseded/`.

## Doctrine

YAI Core is residue-first.

```text
journal = replay/audit
hot state = live session state, not truth
record plane = durable lookup
graph plane = causality and reconstruction
fact plane = analytics and diagnostics
memory plane = consolidated operational experience
projection plane = controlled live cognitive view
reconcile plane = mismatch detection
observability/evaluation = quality facts, not truth
pack material = case-world input to future data planes, not loose files
```

## Planes

| Plane | Target | Role |
|---|---|---|
| hot | hot state / shared memory-ready snapshot | active case, session, projection frame, locks, obligations and deltas |
| journal | append-only files | replay, audit, debug and rebuild |
| record | LMDB | durable normalized record lookup |
| graph | Ladybug | persistent operational causality |
| fact | DuckDB | analytical facts, reports and model behavior analysis |
| memory | Rust engine memory | receipt-backed, graph-derived operational continuity |
| projection | Rust engine projection | live, versioned, delta-aware cognitive view |
| reconcile | Rust engine reconcile | expected-vs-observed mismatch posture |
| observability/evaluation | facts and diagnostics | freshness, trace, provenance, rebuild and quality posture |

Pack material will feed these planes after it has been inspected, validated,
materialized and bound to case-world residue. Packs do not bypass journal,
records, graph, projection, memory or reconcile.

## Linear Delivery

The data-plane roadmap uses the expanded SPINE sequence. Each entry is intended
to be implementable and manually verifiable on its own.

```text
SPINE.23  Hot State Doctrine + ABI                                  done
SPINE.24  Hot State Runtime Snapshot                                done
SPINE.25  Hot State Case Session / Context Integration              done
SPINE.26  Hot State Projection Freshness Integration                 active v1
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
```

## Hot Plane v0

SPINE.23 makes hot state the first live data plane.

```text
hot state is not truth
journal remains replay/audit
YAI_HOME/run/hot-state.json = daemon-owned runtime snapshot
projection freshness = fresh/stale/unknown/rebuilding
stale reason = explicit invalidation posture
```

The v0 implementation is in-process state plus a file-backed, mmap-ready
snapshot contract. True OS shared memory is future work.

SPINE.24 hardens the snapshot lifecycle:

```text
schema = yai.hot_state.v1
write path = hot-state.json.tmp then rename to hot-state.json
missing snapshot = hot_state unavailable / missing_snapshot
corrupt snapshot = hot_state unavailable / invalid_snapshot
valid snapshot = hot_state active
```

SPINE.25 links hot state to the live case lifecycle:

```text
case_session_status = active|inactive|unknown
case_world_status = loaded|not_loaded|unknown
case_context_status = active|inactive|unknown
active_thread_id = current selected interaction thread when known
participant_view_frame_id = current assembled participant view frame when known
decision/receipt changes mark projection stale
projection refresh marks projection fresh
```

SPINE.26 makes freshness consumer-aware:

```text
model/agent stale projection = refresh_required or blocked_for_model
operator/audit/debug stale projection = refresh_recommended
projection inspect exposes freshness_policy and source
prompt path warns when model-visible projection needs refresh
```

## Rules

The journal must remain in the system, but it must recede from the primary
operational path.

SPINE.20 creates the local host root for future data planes:

```text
YAI_HOME/store
```

Future backend-specific subdirectories may be introduced by their owning waves:

```text
YAI_HOME/store/journal
YAI_HOME/store/lmdb
YAI_HOME/store/ladybug
YAI_HOME/store/duckdb
```

Models do not access the case directly. They receive a controlled live
projection assembled with the active interaction thread and authorized memory
or delta material.

Memory is not chat history. It is scoped operational experience derived from
residue.

Pack-derived material is future input:

```text
hot: active pack bindings, authority scopes and projection posture
record: pack manifest, material, binding and validation records
graph: pack materialization, policy, projection and memory-seed edges
fact: usage, policy trigger, review burden, violation and effectiveness facts
memory: pack-derived user/organization method memory
projection: pack-derived model/operator/audit views
```
