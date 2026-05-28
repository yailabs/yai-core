# Current Engineering Status

Status: SPINE.49 Memory / Divergence / Carrier Facts.

## Completed Foundation

NEW.0 through NEW.19 are complete foundation history. SPINE.6A compressed
active engineering docs. SPINE.6B adds the operational extraction contract for
future implementation waves. SPINE.20 adds the local runtime layout. SPINE.20A
rebases the active roadmap so pack materialization becomes the next canonical
spine step. SPINE.21 defines packs as first-class case materialization units.
SPINE.22 freezes filesystem layout, runtime layout, compact engineering docs,
pack doctrine guards and the operational extraction contract before data-plane
implementation begins. SPINE.23 adds the first live hot-state plane. SPINE.23A
expands the linear roadmap and adds the mandatory command surface contract for
future deliveries. SPINE.24 hardens the runtime hot-state snapshot lifecycle.
SPINE.24A maps SPINE.20-SPINE.24 primitives to command views, lab tests and
expected output.
SPINE.25 links that snapshot to case session, case world and case context
lifecycle. SPINE.26 defines the projection freshness policy used by model,
operator, audit and debug consumers. SPINE.27 stabilizes the manual CLI surface
for hot state, doctor diagnostics and projection freshness inspection. SPINE.28
freezes the hot-state block before LMDB record plane work begins. SPINE.28A
renames the canonical core repository to `yai` and the concept-mine/lab
repository to `yai-dev`. SPINE.28B removes local virtualenv roots, archives
README-only ingest placeholders and marks transitional C data shims clearly.
SPINE.29 defines LMDB as durable indexed record lookup under
`YAI_HOME/store/lmdb` without implementing the write path. SPINE.30 adds the
Rust LMDB write path, mirrors daemon-loop journal records into
`records_by_id`, `records_by_case` and `records_by_kind`, and keeps the journal
as replay/audit source. SPINE.31 adds direct record get/list commands over the
LMDB id, case and kind indexes without adding a query planner or journal
fallback reads. SPINE.32 adds subject and receipt indexes as derived views over
stored record envelopes. SPINE.33 freezes the LMDB CLI output shape and manual
operator validation matrix before the record-plane freeze. SPINE.33A adds the
first control/carrier substrate primitives for carrier family, gate outcome,
dispatch status, receipt guarantee mode and host observation posture. SPINE.33B
adds no-execution carrier lanes and route plans so carrier families can be
mapped to dispatch posture without executing effects. SPINE.33C defines
carrier.v1 and adapts the filesystem carrier as the first minimal contract
example. SPINE.33D adds process carrier v0 with test-owned signal control and
blocked arbitrary PID behavior. SPINE.33E adds independent host observation
probe v0, expected/observed process comparison and divergence candidate
posture. SPINE.33F adds the carrier coverage matrix, controlled/observed/imported
mode taxonomy and visible skeleton coverage for non-process carrier families
without adding fake execution. SPINE.33G adds concrete inspectable carrier.v1
skeleton surfaces for network_http, database, repository_git, service,
endpoint, socket, listener, model_provider and review without executing those
effects. SPINE.33H adds a dry-run carrier outcome harness so active, skeleton
and unsupported carriers can represent executed, blocked, deferred, failed,
mismatch, observed, quarantined, waiting_operator, waiting_agent and
not_attempted posture without fake execution. SPINE.33J and SPINE.33K reserve
retrieval, context compiler and model runtime boundaries before SPINE.34:
projection does not disappear, HNSW remains candidate retrieval and MTP belongs
to model runtime / runner decoding strategy. SPINE.33L adds provider runtime /
LAN target surface v0: device registry, runtime target posture, provider start
dry-run plans, provider log paths, model catalog posture and model runtime
status without provider execution. SPINE.33I hardens carrier receipt /
divergence posture by reconciling decision, dispatch, carrier outcome, receipt
posture and observation result into consistent or divergent status. SPINE.33M
adds the Data Context Runtime / RuntimeGraph doctrine: truth lives on durable
planes, computation happens in runtime working sets, Ladybug stores relations,
RuntimeGraph computes over the active case, HNSW finds candidate nodes and
Context Compiler renders controlled views. SPINE.34 freezes the LMDB record
plane as `yai.record.v1` durable indexed lookup with id, case, kind, subject
and receipt indexes, no journal fallback and explicit carrier/control/divergence
record-store validation.
SPINE.35 hardens the journal replay boundary with diagnostic parser inspection,
visible invalid_json, invalid_schema, unsupported_kind and duplicate statuses,
and no LMDB writes.
SPINE.36 implements explicit journal replay to LMDB with dry-run reporting,
basic idempotent replay, invalid input blocking and store query validation.
SPINE.37 hardens replay idempotency with journal identity, schema compatibility,
cursor metadata, replay-status output and persisted replay metadata. SPINE.38
adds durable `yai.replay_report.v1` replay reports with `journal_identity`,
`compatibility`, `cursor_line`, `records_written`, `records_duplicate`,
`invalid_entries` and failed report diagnostics. SPINE.39 freezes the full
journal replay block before graph persistence and RuntimeGraph work.
SPINE.40 defines graph persistence / RuntimeGraph doctrine and schema:
Graph persistence owns durable typed relations and rebuild truth, RuntimeGraph
is the in-memory active case working set, HNSW finds candidate nodes, HNSW is
not graph truth, Context Compiler renders controlled views and Projection does
not disappear. Persistent truth on disk. Computational shape in memory.
HNSW is not graph truth. Projection does not disappear. SPINE.41 adds the first
minimal graph relation write path: `yai.graph_relation.v1` relations are
derived from LMDB records, written to `lmdb_graph_relations_v0` and inspected
with `yai graph materialize --case <case_ref>` and
`yai graph relations --case <case_ref>`. RuntimeGraph remains planned.
Ladybug integration remains future. SPINE.42 adds an active-minimal
RuntimeGraph in-memory working set loaded from graph relations with
`yai graph runtime-load --case <case_ref>`. The working set is
per-command ephemeral, reports nodes, edges, incoming/outgoing adjacency,
`resident_service: planned`, `hnsw: future_candidate_index` and
`context_compiler: future_consumer`. Graph persistence is durable truth.
SPINE.43 adds RuntimeGraph rebuild from journal and existing graph relations.
The journal path rebuilds through LMDB replay, graph relations and
RuntimeGraph load; the graph-relations path loads from existing typed
relations. Rebuild emits `yai.runtime_graph_rebuild_report.v1`,
`runtime_graph_rebuild` output and `runtime-summary` proof. RuntimeGraph is
not durable truth. RuntimeGraph is not durable truth and resident service
planned remains the runtime posture. The active lab path is
`labs/filesystem-loop`.
SPINE.44 adds RuntimeGraph fanout, fanin, neighborhood and causal path
diagnostics. RuntimeGraph is not a generic graph database; query is bounded
traversal over the active-case working set. The edge-kind filter applies to
fanout, fanin and neighborhood. Path diagnostics use max-depth and return
`path_status: not_found` when no causal path exists. Plain output remains
parseable and color-aware graph inspection remains future console/studio
visual doctrine.
SPINE.44A adds the active operator review / deferred action loop v0 for
filesystem sandbox writes. `require_review` creates a `pending_operator` item;
approve may execute the safe reviewed filesystem write, while deny, defer and
quarantine keep `carrier_attempted: false` and `execution_performed: false`.
Automation/test mode emits `review_required: yes`, `status:
pending_operator` and `next_commands`, then exits without waiting for operator
input.
`subject:linenoise-terminal is prompt surface`; operator reviewer authority is
separate and local-dev scoped.
SPINE.44B adds the CLI review interaction surface over that durable state:
`control pending` and `control show` are automation views, `control review
--interactive` is TTY-scoped and returns `not_a_tty` without a terminal,
`control watch` polls state changes and `control wait` supports timeout-based
automation. `next_commands` makes the pending queue directly scriptable.
`subject:operator-reviewer is review authority`.
SPINE.44C aligns the review loop test matrix across `labs/filesystem-loop`
and `labs/filesystem-loop`. The matrix proves blocked, `pending_operator`,
approve, deny, defer, quarantine, `next_commands`, `wait timeout`, bounded
`watch` and non-TTY behavior. Non-execution paths keep
`carrier_attempted: false` and `execution_performed: false`. Model-facing
posture is explicit: model proposal observed, model cannot approve, and
automatic proposed-op gate import is future work.
SPINE.45 freezes graph schema, `yai.graph_relation.v1`, graph materialize,
graph relations, RuntimeGraph runtime-load/runtime-summary, runtime graph
rebuild, `yai.runtime_graph_rebuild_report.v1`, fanout, fanin, neighborhood,
causal path, bounded traversal, edge-kind filter, path found, path not_found
and empty case. RuntimeGraph is not durable truth. Review/control records are
graph-visible when actual refs exist.
SPINE.46 defines DuckDB as the analytical fact plane with `yai.fact.v1`,
`yai facts status`, `yai facts schema` and `yai facts init`. A fact is not just
a row. A fact is a temporally scoped, provenance-bearing assertion with
`transaction_time`, `valid_time_start`, `valid_time_end`, `known_at`,
`revision_of`, `superseded_by` and `retracted_by` posture. Facts are not truth:
they cannot authorize, execute, approve, deny, mutate operational truth or
replace evidence. SPINE.46 initializes the twelve planned fact tables and keeps
`facts_extracted: 0`; No fact extraction occurs until SPINE.47.
SPINE.47 implements idempotent extraction for `fact_receipt`, `fact_decision`
and `fact_projection` from LMDB records into DuckDB. Fact extraction is
derivation, not migration. Facts are not truth. `yai facts extract --case
<case_ref> --kind receipt|decision|projection|core` and `yai facts summary
--case <case_ref>` are active. Fact IDs are deterministic fact IDs using
`fact:<kind>:<source_record_id>`. Bitemporal fields are populated, revision
fields remain empty and valid_time_end sentinel: 0 means open-ended. No fact
revision is implemented yet.
SPINE.48 implements derived analytical extraction for `fact_model_behavior`
and `fact_policy_outcome`. Model behavior facts measure model output posture,
including conservative `authority_overclaim`, `unsupported_claim`,
`review_required` and filesystem proposal markers from structured records or
stable text markers. Policy outcome facts measure policy/control posture from
decision, review/control and policy records. `yai facts extract --case
<case_ref> --kind model_behavior|policy_outcome|behavior|all` is active.
`core` remains receipt + decision + projection; `behavior` is model_behavior +
policy_outcome; `all` is core + behavior. Facts are not truth. model proposal
is not execution, model cannot approve, and automatic proposed-op gate import
is future work. No LLM-based classification is used.

Current:

```text
SPINE.20 Local Runtime Layout completed.
SPINE.20A Pack Roadmap Rebase completed.
SPINE.21 Pack Materialization Doctrine completed.
SPINE.22 Filesystem & Runtime Layout Freeze completed.
SPINE.23 Hot State Doctrine + ABI completed.
SPINE.23A Roadmap Expansion + Command-Test Contract completed.
SPINE.24 Hot State Runtime Snapshot completed.
SPINE.24A Retroactive Command Surface Recovery completed.
SPINE.25 Hot State Case Session / Context Integration completed.
SPINE.26 Hot State Projection Freshness Integration completed.
SPINE.27 Hot State CLI + Manual Validation completed.
SPINE.28 Hot State Freeze completed.
SPINE.28A Repository Identity Cutover completed.
SPINE.28B Internal Source Surface Cleanup completed.
SPINE.29 LMDB Record Plane Doctrine + Schema completed.
SPINE.30 LMDB Record Write Path completed.
SPINE.31 LMDB Record Read / Query Path completed.
SPINE.32 LMDB Case / Subject / Receipt Indexes completed.
SPINE.33 LMDB CLI + Manual Validation completed.
SPINE.33A Control / Carrier Substrate Primitives completed.
SPINE.33B Operation Dispatch + Multiplex v0 completed.
SPINE.33C Carrier Contract v1 + Filesystem Adapter completed.
SPINE.33D Process Carrier v0 / Signal Control completed.
SPINE.33E Host Observation Probe v0 / Bypass Verification completed.
SPINE.33F Carrier Coverage Matrix + Mode Taxonomy completed.
SPINE.33G Non-Process Carrier Skeletons completed.
SPINE.33H Carrier Outcome Harness completed.
SPINE.33I Carrier Receipt / Divergence Hardening completed.
SPINE.33J Retrieval and Model Runtime Roadmap Rebase completed.
SPINE.33K Context Compiler / Retrieval / MTP Roadmap Correction completed.
SPINE.33L Provider Runtime / LAN Target Surface v0 completed.
SPINE.33M Data Context Runtime / RuntimeGraph Doctrine completed.
SPINE.34 LMDB Record Plane Freeze completed.
SPINE.35 Journal Replay Doctrine + Parser Hardening completed.
SPINE.36 Journal Replay to LMDB completed.
SPINE.37 Replay Idempotency + Schema Version Handling completed.
SPINE.38 Replay Diagnostics / Rebuild Report completed.
SPINE.39 Journal Replay Freeze completed.
SPINE.40 Graph Persistence / RuntimeGraph Doctrine + Schema completed.
SPINE.41 Graph Relation Write Path completed.
SPINE.42 RuntimeGraph In-Memory Working Set completed.
SPINE.43 RuntimeGraph Rebuild from Journal / LMDB / Graph Store completed.
SPINE.44 RuntimeGraph Query / Causal Path / Diagnostics completed.
SPINE.44A Operator Review / Deferred Action Loop v0 completed.
SPINE.44B CLI Review Interaction Surface completed.
SPINE.44C Review Loop Test Matrix + Lab Alignment completed.
SPINE.45 Graph + RuntimeGraph Freeze completed.
SPINE.45A Documentation Root Canon Collapse completed.
SPINE.45B Case Runtime Semantics / Retrieval Federation / Context Residency Roadmap Rebase completed.
SPINE.46 DuckDB Fact Plane Doctrine + Bitemporal Schema completed.
SPINE.47 Receipt / Decision / Projection Facts completed.
SPINE.48 Model Behavior / Policy Outcome Facts completed.
REPO.HYGIENE.0 Header / Ownership Standard + Agent Operating Appendix completed.
REPO.HYGIENE.1 Apply Headers + Source/Docs Surface Cleanup completed.
```

Next:

```text
SPINE.47 Receipt / Decision / Projection Facts completed. The first extraction
path derives fact_receipt, fact_decision and fact_projection from LMDB records
with deterministic IDs and duplicate-aware idempotency. Facts remain
non-authoritative analytical assertions.

SPINE.48 Model Behavior / Policy Outcome Facts completed. Behavior extraction
derives fact_model_behavior and fact_policy_outcome with duplicate-aware
idempotency. Facts remain non-authoritative analytical assertions: model
proposal is not execution, model cannot approve and automatic proposed-op gate
import is future work.

SPINE.49 Memory / Divergence / Carrier Facts completed.

SPINE.49 derives `fact_carrier_outcome`, `fact_divergence` and
`fact_memory_quality` from LMDB records. carrier facts measure carrier posture;
divergence facts are not reconcile action; memory facts are not memory. Facts
are not truth. `memory_is_truth: false` is visible for memory quality
extraction. The group `operational` means carrier_outcome + divergence +
memory_quality; `all` means core + behavior + operational. Extraction is
bitemporal and idempotent extraction.
```

Guard vocabulary: control pending, control show, control review --interactive,
control watch, control wait, next_commands, not_a_tty, pending_operator,
approve, deny, defer, quarantine, carrier_attempted: false,
execution_performed: false, wait timeout, watch, model proposal observed,
model cannot approve, automatic proposed-op gate import is future work,
subject:linenoise-terminal is prompt surface, subject:operator-reviewer is
review authority, labs/filesystem-loop, labs/filesystem-loop.

Foundation status:

```text
filesystem refactor closed
runtime layout exists
active docs compact
pack doctrine guarded
operational extraction contract active
hot state v0 active
expanded linear roadmap active
command surface contract active
command surface inventory active
hot-state snapshot v1 active
hot-state snapshot missing/corrupt handling active
hot-state session/context lifecycle fields active
projection freshness policy active
consumer-aware model/operator freshness posture active
hot-state CLI manual validation active
hot-state block frozen
repository identity cutover active
source surface cleanup active
LMDB record-plane doctrine active
record store status surface active
LMDB record write path active
record store summary surface active
LMDB record read/query surface active
LMDB subject/receipt indexes active
LMDB CLI manual validation active
LMDB record-plane freeze active
journal replay parser hardening active
journal inspect diagnostics active
journal replay to LMDB active
replay idempotency/schema metadata active
idempotent replay v0 active
replay diagnostics report active
journal replay freeze active
graph persistence / RuntimeGraph schema active
graph relation write path active_minimal
RuntimeGraph working set active_minimal
control/carrier substrate primitives active
host observation probe active
carrier coverage matrix active
carrier outcome harness active
carrier receipt divergence hardening active
non-process carrier skeletons active
context compiler retrieval doctrine active
Data Context Runtime doctrine active
RuntimeGraph active case working set doctrine active
RuntimeGraph resident service planned
RuntimeGraph remains planned for implementation
model runtime MTP doctrine active
provider runtime LAN target surface active
device registry v0 active
file/header ownership standard active
agent operating appendix active
principal ownership headers and READMEs active
```

## Current Layout

```text
include/
system/
engine/
cmd/
proto/
tests/
docs/
tools/
packaging/
examples/
vendor/
```

## Local Runtime Layout

```text
PREFIX=$HOME/.local
YAI_HOME=$HOME/.yai
$(PREFIX)/bin/yai
$(PREFIX)/bin/yaid
$(YAI_HOME)/run
$(YAI_HOME)/store
$(YAI_HOME)/store/lmdb
$(YAI_HOME)/log
$(YAI_HOME)/tmp
$(YAI_HOME)/cases
$(YAI_HOME)/sockets
$(YAI_HOME)/config
$(YAI_HOME)/run/yaid.sock
$(YAI_HOME)/run/hot-state.json
```

`build/tmp` remains test/lab space. `YAI_HOME` is the local runtime home.

Removed active implementation roots:

```text
lib/
crates/
daemon/
ctl/
```

## Active Engineering Docs

```text
README.md
command-surface.md
current-status.md
data-context-runtime-runtimegraph.md
data-plane-roadmap.md
extraction-plan.md
filesystem-target.md
four-repo-roadmap.md
hot-state-plane.md
lmdb-record-plane.md
lmdb-record-plane-freeze.md
journal-replay-parser-hardening.md
journal-replay-to-lmdb.md
pack-format.md
pack-roadmap.md
testing.md
wave-template.md
operational-extraction-contract.md
source-surface.md
```

## Known Caveats

The current data plane remains partly journal-backed and partly transitional C
smoke support. SPINE.23 implements hot-state semantics and a daemon-owned
`YAI_HOME/run/hot-state.json` snapshot, not durable truth. SPINE.24 makes the
snapshot schema `yai.hot_state.v1`, writes through temp-and-rename, and makes
`yai hot status` handle missing and corrupt snapshots. SPINE.25 makes the
snapshot carry `case_session_status`, `case_world_status`,
`case_context_status`, `active_thread_id` and `participant_view_frame_id`.
SPINE.26 adds consumer-aware freshness policy so model/agent views are stricter
than operator, audit and debug views. SPINE.27 makes `yai hot status`,
`yai doctor` and `yai projection inspect` the stable manual inspection surface.
SPINE.28 freezes this surface as a non-authoritative live cache boundary.
True OS shared memory/mmap, expanded LMDB indexes, Ladybug, DuckDB, projection deltas,
memory consolidation, cross-plane reconcile and observability/evaluation facts remain
future SPINE.33B-SPINE.80 work. Pack
material is future data-plane input, but SPINE.21 does not implement pack
records or backends. SPINE.20 creates `YAI_HOME/store` as the future durable
data-plane root but does not create those backends.

LMDB begins at SPINE.29 as the durable record lookup plane. It will not replace
hot state. Hot state remains the live liveness/freshness surface and may be
rebuilt or refreshed from durable residue.

`yai store status` reports `record_store_backend: lmdb`,
`record_store_status: missing|not_initialized|ready|unavailable` and
`record_store_path: <YAI_HOME>/store/lmdb`. `ready` means the LMDB environment
opened and schema metadata exists. `yai store summary` reports aggregate
counts, including subject and receipt index counts. `yai store record get
<record_id>` and `yai store record list --case`, `--kind`, `--subject` or
`--receipt` read from LMDB only; missing or uninitialized LMDB reports status
instead of fabricating journal-backed records.

Source surface:

```text
system/ is not a second data engine
engine/ is the Rust data spine target
system/{store,graph,index,memory,projection,reconcile} are transitional C shims
system/ingest and include/yai/ingest are archived placeholders until ingest work
```

Local virtualenv roots are ignored and blocked:

```text
venv/
.venv/
env/
ENV/
```

Repository identity:

```text
YAI/yai     = canonical local AI operational control system
YAI/yai-dev = development lab, concept mine, harness and scenario workspace
```

The local directory rename is complete. Remote URLs are intentionally left
unchanged until the GitHub repositories are renamed or confirmed.

`yai-dev` residue for SPINE.21 was read-only inspected. No yai-dev source file
was modified in that wave. The extraction inventory records
pack/materialization classifications.

yai-dev residue for SPINE.23 was read-only inspected and classified in the
extraction inventory. No yai-dev source file was modified.

yai-dev residue for SPINE.24 was read-only inspected and classified in the
extraction inventory. No yai-dev source file was modified.

yai-dev command/runtime residue for SPINE.24A was read-only inspected and
classified in the extraction inventory. No yai-dev source file was modified.

yai-dev residue for SPINE.25 was read-only inspected and classified in the
extraction inventory. No yai-dev source file was modified.

yai-dev residue for SPINE.26 was read-only inspected and classified in the
extraction inventory. No yai-dev source file was modified.

yai-dev command/runtime residue for SPINE.27 was read-only inspected and
classified in the extraction inventory. No yai-dev source file was modified.

yai-dev hot-state/session/view residue for SPINE.28 was verified against
SPINE.23-SPINE.27 classifications. No yai-dev source file was modified.

SPINE.28A updates active repo identity. `yai-dev` has a role note and remains
the source for wave-coupled concept mining.

SPINE.28B is internal `yai` source hygiene only. No `yai-dev` source file was
modified.

SPINE.29 read-only inspected yai-dev store, record, query, view, evidence and
execution receipt residue. No `yai-dev` source file was modified.

SPINE.30 read-only inspected yai-dev store, record, query and execution receipt
residue for write-path posture. No `yai-dev` source file was modified.

SPINE.31 read-only inspected yai-dev query, index, store and records residue
for read/query posture. No `yai-dev` source file was modified.

SPINE.32 read-only inspected yai-dev index, query, records, store and
operational receipt residue for subject/receipt index posture. No `yai-dev`
source file was modified.

SPINE.33 read-only inspected yai-dev query, index and store residue for CLI
lookup wording, zero-result posture and no-fallback query boundaries. No
`yai-dev` source file was modified.

SPINE.33A read-only inspected yai-dev runtime carrier, execution,
observation, decision, boundary subject and queue/review residue for
control/carrier posture. No `yai-dev` source file was modified.

SPINE.33B read-only inspected yai-dev carrier loop, queue gate/view, execution
stage/enforcement and subject boundary residue for dispatch/multiplex posture.
No `yai-dev` source file was modified.

SPINE.33C read-only inspected yai-dev host-mediated/native carrier,
controlled action, operational receipt, execution record and decision/receipt
contract residue for carrier.v1 posture. No `yai-dev` source file was modified.

SPINE.33D read-only inspected yai-dev command execution, process observation,
stage/enforcement, recovery and watch-session residue for process carrier
signal posture. No `yai-dev` source file was modified.

SPINE.33E read-only inspected yai-dev process observer, runtime probe, watch
plan/session, observation view, host-mediated/native probe and recovery residue
for host observation and bypass mismatch posture. No `yai-dev` source file was
modified.

SPINE.33F read-only inspected yai-dev boundary subject, provider, observation
subject and carrier residue for carrier family coverage, mode taxonomy and
skeleton posture. No `yai-dev` source file was modified.

SPINE.33G read-only inspected yai-dev database, endpoint, listener, service
and socket boundary subjects, provider request/result and transport material,
and subject observers for non-process skeleton posture. No `yai-dev` source
file was modified.

SPINE.33H read-only inspected yai-dev execution error, operational receipt,
stage state, decision-receipt boundary, observation transition and
decision/conflict residue for outcome, receipt posture and divergence-candidate
concepts. No `yai-dev` source file was modified.

SPINE.33I read-only inspected yai-dev receipt consistency, decision-receipt
boundary, error envelope, recovery hint, conflict resolution and transition
mismatch residue for carrier consistency posture. No `yai-dev` source file was
modified.

SPINE.33J and SPINE.33K completed roadmap/doctrine rebases only. They reserve
Vector Retrieval / HNSW as candidate retrieval, not graph/memory/decision
authority; define Context Compiler as the system that assembles compiled
projection/model context; and reserve Model Runtime / Runner plus MTP policy
without claiming YAI is a universal inference engine.

SPINE.33L read-only inspected yai-dev provider, transport, machine, residency,
activation and runtime registry residue for provider lifecycle, provider
daemon, local/remote transport, machine authorization, runtime target, artifact
layout and startup plan concepts. No `yai-dev` source file was modified.

Future implementation waves must classify corresponding yai-dev residue. A
wave is not complete until old material has been absorbed, rewritten, split,
externalized, quarantined, marked compatibility-only, marked delete-later, left
untouched with a reason, or explicitly deferred.

Archived historical records live under:

```text
work/archive/legacy-docs/engineering/
```
