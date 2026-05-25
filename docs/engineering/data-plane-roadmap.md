# Data Plane Roadmap

This is the active data-plane roadmap. It replaces older active-looking data
spine and operational data-plane notes, which are archived under
`docs/archive/engineering/roadmaps/` and `docs/archive/engineering/superseded/`.

## Doctrine

YAI is residue-first.

```text
journal = replay/audit
hot state = live session state, not truth
record plane = durable lookup
graph plane = causality and reconstruction
runtime graph = active in-memory case graph / working set, not truth
fact plane = analytics and diagnostics
retrieval plane = retrieval units and candidate retrieval, not truth
memory plane = consolidated operational experience
projection plane = controlled live cognitive view compiled for consumers
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
| graph | Ladybug / graph persistence | persistent operational causality, relation truth and rebuild contract |
| runtime graph | RuntimeGraph / Data Context Runtime | active in-memory case graph / working set loaded from durable truth |
| fact | DuckDB | analytical facts, reports and model behavior analysis |
| retrieval | Context Compiler / HNSW optional accelerator | scoped retrieval units, candidate retrieval, RuntimeGraph expansion and context narrowing |
| memory | Rust engine memory | receipt-backed, graph-derived operational continuity |
| projection | Rust engine projection / compiled context | live, versioned, delta-aware cognitive view for consumers |
| reconcile | Rust engine reconcile | expected-vs-observed mismatch posture |
| observability/evaluation | facts and diagnostics | freshness, trace, provenance, rebuild and quality posture |

Pack material will feed these planes after it has been inspected, validated,
materialized and bound to case-world residue. Packs do not bypass journal,
records, graph, projection, memory or reconcile.

## Data Context Runtime

Truth lives on durable planes. Computation happens in runtime working sets.

```text
LMDB stores records.
Ladybug stores relations.
DuckDB stores facts.
RuntimeGraph computes over the active case.
HNSW finds candidate nodes.
Context Compiler renders controlled views.
```

RuntimeGraph is the Data Context Runtime working graph for an active
case/session. It is not a replacement for LMDB, Ladybug or DuckDB. It is loaded
or rebuilt from durable truth and updated during active work so graph traversal,
retrieval candidate expansion, policy filtering and context assembly can be
performed quickly.

HNSW is attached to this runtime substrate as a rebuildable proximity index over
retrieval units. The roadmap names this bridge explicitly:

```text
SPINE.58D HNSW Candidate -> RuntimeGraph Expansion
```

HNSW finds vectors. RuntimeGraph gives meaning. Context Compiler emits the
compiled projection/model context; ContextFrame is a temporary compiled
artifact, not truth.

## Linear Delivery

The data-plane roadmap uses the expanded SPINE sequence. Each entry is intended
to be implementable and manually verifiable on its own.

```text
SPINE.23  Hot State Doctrine + ABI                                  done
SPINE.24  Hot State Runtime Snapshot                                done
SPINE.25  Hot State Case Session / Context Integration              done
SPINE.26  Hot State Projection Freshness Integration                 done
SPINE.27  Hot State CLI + Manual Validation                         done
SPINE.28  Hot State Freeze                                          done
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
SPINE.39  Journal Replay Freeze                                     planned

SPINE.40  Graph Persistence / RuntimeGraph Doctrine + Schema         planned
SPINE.41  Graph Relation Write Path                                  planned
SPINE.42  RuntimeGraph In-Memory Working Set                         planned
SPINE.43  RuntimeGraph Rebuild from Journal / LMDB / Graph Store     planned
SPINE.44  RuntimeGraph Query / Causal Path / Diagnostics             planned
SPINE.45  Graph + RuntimeGraph Freeze                                planned

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

SPINE.27 stabilizes the manual command surface:

```text
yai hot status = hot-state snapshot, lifecycle and freshness status
yai doctor = runtime path and hot-state schema/readability diagnostic
yai projection inspect = journal projection plus freshness policy/source when available
```

Current command views:

```text
yai hot status
yai doctor
make smoke-spine24a
```

`docs/engineering/command-surface.md` maps the hot-state primitive, snapshot
schema, missing/corrupt snapshot handling and daemon-backed status path to
manual commands and expected output.

SPINE.28 freezes the hot-state block:

```text
hot state remains non-authoritative
snapshot schema remains yai.hot_state.v1
doctor/hot/projection command surface remains stable
missing/corrupt/valid snapshot behavior remains explicit
case_session/case_world/case_context/participant fields remain diagnostic cache
freshness_policy remains the model/operator freshness posture field
```

LMDB boundary:

```text
Hot state is the live cache and may be rebuilt or refreshed from durable residue.
SPINE.29 begins the durable record plane.
LMDB will not replace hot state.
LMDB will become durable lookup; hot state remains current liveness/freshness surface.
```

## Record Plane v0

SPINE.29 defines the durable record lookup plane:

```text
LMDB is durable indexed record lookup
hot state is not LMDB
journal remains replay/audit
record_env under YAI_HOME/store/lmdb
schema = yai.record.v1
record:id / record:case / record:kind / record:subject key grammar
```

SPINE.29 adds `yai store status` and doctor fields:

```text
record_store_backend: lmdb
record_store_status: missing|not_initialized|unavailable
record_store_path: <YAI_HOME>/store/lmdb
```

`ready` is not a SPINE.29 status.

SPINE.30 LMDB Record Write Path adds:

```text
LMDB crate dependency in the Rust engine
open/create record_env at YAI_HOME/store/lmdb
write records_by_id
write records_by_case
write records_by_kind
schema_meta/meta:schema = yai.record.v1
yai store summary = aggregate counts only
daemon loops mirror their journal records into LMDB after journal write succeeds
```

Journal remains replay/audit. LMDB write errors are explicit command failures
after the daemon loop response has preserved the journal path.

SPINE.31 LMDB Record Read / Query Path adds:

```text
yai store record get <record_id>
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
records_by_id read path
records_by_case -> records_by_id listing
records_by_kind -> records_by_id listing
missing record returns record: not_found
missing or uninitialized LMDB reports status instead of fake journal reads
```

It does not add subject, receipt, projection or time indexes. Subject and
receipt indexes are added by SPINE.32; projection and time remain later work.

SPINE.32 LMDB Case / Subject / Receipt Indexes adds:

```text
records_by_subject
records_by_receipt
record:subject:<subject_ref>:<record_id> -> record_id
record:receipt:<receipt_ref>:<record_id> -> record_id
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
records_by_subject and records_by_receipt summary counts
missing subject or receipt query returns records_total: 0
```

These indexes are derived lookup views over structured record fields. They are
not new truth and they do not create a journal fallback read path.

SPINE.33 LMDB CLI + Manual Validation freezes:

```text
yai store status
yai store summary
yai store record get <record_id>
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
```

The list output shape is `filter`, `filter_value`, `records_total`, `limit`
and `records`. Missing records return `record: not_found`; zero-result lists
return `records: none`; missing or uninitialized LMDB reports record-store
status and does not fall back to the journal.

SPINE.34 LMDB Record Plane Freeze freezes:

```text
schema = yai.record.v1
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
no journal fallback
carrier_request / effect_receipt / divergence record-store posture
```

The freeze validates that carrier/control/divergence records can be preserved
and queried by the frozen indexes. It adds no new command surface and no
journal replay behavior.

SPINE.35 Journal Replay Doctrine + Parser Hardening adds:

```text
yai journal inspect --path <journal.jsonl> [--show-errors]
diagnostic parser policy
invalid_json
invalid_schema
unsupported_kind
duplicate
replay diagnostics
replay_ready: yes|no
```

Journal is replay/audit. LMDB is durable indexed record lookup. Journal inspect
does not write LMDB, does not rebuild LMDB and does not create a no silent
journal fallback exception. Duplicate detection prepares later idempotency
work, but full replay waits for SPINE.36.

SPINE.36 Journal Replay to LMDB adds:

```text
yai journal replay --path <journal.jsonl>
yai journal replay --path <journal.jsonl> --dry-run
records_seen
records_written
records_duplicate
records_skipped
idempotent
```

Journal replay to LMDB uses the frozen LMDB record/index path and writes
`yai.record.v1` envelopes into records_by_id, records_by_case, records_by_kind,
records_by_subject and records_by_receipt. Dry-run writes nothing. Invalid
journal input blocks replay with no silent skip. Existing LMDB records are
reported as records_duplicate so a second replay is idempotent. Store queries
continue to have no journal fallback.

SPINE.37 Replay Idempotency + Schema Version Handling adds:

```text
yai journal replay-status --path <journal.jsonl>
journal_identity
record_schema
journal_schema
compatibility
cursor_line
replay_status
```

Replay metadata is stored under `meta:replay:<journal_identity>` in the LMDB
metadata namespace. Current replay accepts `yai.store.record.v0` journal input
and materializes `yai.record.v1` records. Schema mismatch rejects replay with
`records_written: 0`; missing schema is invalid unless a future converter
explicitly reports `compat_legacy`.

SPINE.38 Replay Diagnostics / Rebuild Report adds:

```text
yai journal replay-report --path <journal.jsonl>
yai.replay_report.v1
journal_identity
compatibility
cursor_line
records_written
records_duplicate
invalid_entries
failed report
```

Replay reports are durable operational evidence for journal to LMDB rebuilds.
They live under `YAI_HOME/store/replay/reports/` and describe completed,
idempotent and failed materialization attempts.

SPINE.33A Control / Carrier Substrate Primitives adds:

```text
carrier family
gate outcome
dispatch status
receipt guarantee mode
host observation posture
yai carrier families
```

It is a control/effect/observation substrate wave, not an LMDB behavior change.
It makes decision not execution, carrier not free tool invocation, observation
not enforcement and receipt not logging explicit before dispatch work begins.

SPINE.33B Operation Dispatch + Multiplex v0 adds:

```text
dispatch lane
dispatch plan
dispatch multiplex route helper
yai carrier lanes
yai carrier route --family <family>
```

Dispatch remains separate from decision and execution. Carrier lanes are
inspectable route targets, not a single global queue, and every SPINE.33B route
reports `execution_performed: false`.

SPINE.33C Carrier Contract v1 + Filesystem Adapter adds:

```text
carrier.v1
carrier outcome
carrier receipt posture
filesystem carrier contract mapping
yai carrier inspect filesystem
```

Filesystem becomes the first active-minimal carrier.v1 adapter. Read maps to
observed, blocked write maps to blocked, allowed write maps to executed and the
guarantee mode is documented as interposed.

SPINE.33D Process Carrier v0 / Signal Control adds:

```text
process carrier.v1
process observe
process signal dry-run
test-owned TERM/KILL smoke
unsafe target blocking
```

Process signals are controlled effects, not shell commands. Real signal effects
are limited to test-owned child processes in smoke/lab tests.

SPINE.33E Host Observation Probe v0 / Bypass Verification adds:

```text
host probe process observation
expected/observed comparison
divergence_candidate posture
yai observe process
yai observe compare-process
```

Observation is not enforcement. A mismatch produces a divergence candidate and
no silent repair.

Source surface boundary:

```text
system/{store,graph,index,memory,projection,reconcile} are transitional C shims.
engine/yai-engine is the future data-plane owner.
LMDB work must not treat system/store as final durable record ownership.
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
