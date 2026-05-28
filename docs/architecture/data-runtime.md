# Data Runtime

YAI separates durable truth from runtime computation.

```text
durable truth: journal, LMDB records, graph relations
runtime computation: hot state, RuntimeGraph, projections
analytics: DuckDB facts
planned retrieval/context: HNSW/retrieval, memory, Context Compiler
```

## Planes

- Hot state is a live cache and snapshot, not truth.
- Journal is append-only replay and audit residue.
- LMDB is durable indexed record lookup.
- Graph persistence stores durable typed relations as `yai.graph_relation.v1`.
- RuntimeGraph loads active-case graph relations into a per-command in-memory
  working set.
- RuntimeGraph rebuild can replay journal material into LMDB, materialize graph
  relations and load a runtime graph report.
- DuckDB stores analytical, temporal, provenance-bearing facts as
  `yai.fact.v1`.

RuntimeGraph is not durable truth. It is bounded active-case computation over
relations that can be rebuilt from durable planes.

## Fact Plane

Journal records chronology. LMDB stores record truth. Graph persistence stores
relation truth. RuntimeGraph computes over active case relations. DuckDB stores
analytical, temporal, provenance-bearing facts derived from records, graph
relations, receipts, decisions, projections, carriers, model behavior, replay
reports, runtime graph diagnostics, retrieval and memory.

A fact is not just a row. A fact is a temporally scoped,
provenance-bearing assertion with
`transaction_time`, `valid_time_start`, `valid_time_end`, `known_at`,
`revision_of`, `superseded_by` and `retracted_by` posture.

Facts are not truth. Facts can explain, score, measure, compare, filter and
report. Facts cannot authorize, execute, approve, deny, mutate operational
truth or replace evidence.

The SPINE.46 fact schema is `yai.fact.v1` and initializes these DuckDB tables:

```text
fact_receipt
fact_decision
fact_projection
fact_carrier_outcome
fact_divergence
fact_replay
fact_runtime_graph
fact_model_behavior
fact_policy_outcome
fact_memory_quality
fact_retrieval_quality
fact_provider_runtime
```

No fact extraction happens in SPINE.46. Fact extraction begins in SPINE.47.

SPINE.47 starts real fact extraction from LMDB records into DuckDB for
`fact_receipt`, `fact_decision` and `fact_projection`. Fact extraction derives
analytics from LMDB records; it is derivation, not migration. Records remain
truth in LMDB and facts remain non-authoritative.

Extracted fact rows use deterministic fact IDs:

```text
fact:<kind>:<source_record_id>
```

Extraction is idempotent: a second extraction reports duplicates instead of
inserting duplicate rows. For SPINE.47, `status` is `current`,
`transaction_time` and `known_at` are extraction time, `valid_time_start` is
the source record time when available and extraction time otherwise, and the
valid_time_end sentinel: 0 means open-ended. Revision fields
`revision_of`, `superseded_by` and `retracted_by` remain empty because there is
No fact revision or supersession implementation yet.

SPINE.48 extends derived facts to `fact_model_behavior` and
`fact_policy_outcome`. Model behavior facts measure model output posture;
policy outcome facts measure policy/control posture. Facts are not truth.
model proposal is not execution. model cannot approve. automatic proposed-op
gate import is future work. `authority_overclaim`, `unsupported_claim`,
`review_required` and `policy_outcome` remain analytical fields. The extractor
uses structured fields and stable markers only; No LLM-based classification is
used. `core` remains receipt + decision + projection. `behavior` is
model_behavior + policy_outcome. `all` is core + behavior.

SPINE.49 extends derived facts to `fact_carrier_outcome`, `fact_divergence` and
`fact_memory_quality`. carrier facts measure carrier posture. divergence facts
are not reconcile action. memory facts are not memory. Facts are not truth.
Extraction stays bitemporal and idempotent extraction. `operational` is
carrier_outcome + divergence + memory_quality. `all` is core + behavior +
operational.

SPINE.50 adds compact fact reports with `yai facts report --case <case_ref>`
and section views for `--section receipts`, `--section decisions`,
`--section projections`, `--section policy`, `--section carriers`,
`--section divergence`, `--section memory` and `--section model`. `facts
summary` is counts; `facts report` is interpreted explanatory sections over
existing DuckDB facts only. facts reports are not truth, not audit packets, not
reconcile actions and not memory consolidation. Zero divergence/model facts are
valid when source records are absent: `none_observed` and `no_model_records`.
Memory reports include `memory_is_truth: false`.

SPINE.51 freezes Fact Plane v1. DuckDB `yai.fact.v1` remains a derived
analytical layer over existing records, graph relations, projections, receipts,
decisions, carriers, memory candidates and runtime diagnostics. The freeze
does not add fact tables or extraction classes.

The frozen first block includes `fact_receipt`, `fact_decision`,
`fact_projection`, `fact_carrier_outcome`, `fact_divergence`,
`fact_model_behavior`, `fact_policy_outcome` and `fact_memory_quality`.
`transaction_time`, `valid_time_start`, `valid_time_end`, `known_at`,
`revision_of`, `superseded_by` and `retracted_by` remain part of the
bitemporal/revision posture. Status posture includes current, superseded,
retracted, stale, contested, historical_only, branch_only, counterfactual,
needs_review and unknown.

`facts summary` and `facts report` preserve `facts_are_truth: false`; memory
sections preserve `memory_is_truth: false`. Zero divergence/model facts are
valid when source records are absent and report `none_observed` and
`no_model_records`. Fact extraction remains idempotent extraction.

Hard distinction:

```text
fact != record
fact != receipt
fact != decision
fact != policy
fact != memory
fact != graph relation
fact != projection
fact != retrieval candidate
fact != ContextFrame
```

## Planned Planes

HNSW and retrieval are planned candidate-selection surfaces, not graph truth.
Native retrieval, attached retrieval and federated retrieval produce retrieval
candidates. Retrieval candidates are not evidence until resolved. External
retrieval cannot enter model context without receipt/scope/provenance gating,
and external retrieval cannot become case material without explicit import.

Context Compiler is planned as a controlled rendering surface over case
material, not a replacement for projection, policy or memory. ContextFrame is
not truth. ContextDelta is not memory. CaseModelSession is not authority. KV
cache is not YAI memory.

Graph corrections are planned as append-only patches, not destructive edits.
Temporal Graph Revision, Graph Patch Operation, Dependency Closure and
Counterfactual case branches must remain stale-aware so projections, context,
memory, retrieval and reconcile can invalidate dependent material correctly.

## Runtime-Resolved Case Boundary

SPINE.51B introduces runtime-resolved CaseHandle, SubjectHandle,
AuthorityScope, VisibilityScope, ResourceScope and CapabilityLease posture over
existing records, journal replay material, graph/fact posture and filesystem
sandbox conventions.

This does not make facts, projections or graph relations authority. refs are
identifiers, not authority. bindings are relations, not capabilities. The data
runtime supplies material for `case resolve`, `case scope` and `capability
derive`; the lease is the bounded operation permission.

Future Live Projection Frame Schema should consume resolved VisibilityScope and
AuthorityScope instead of reverse-engineering access from summaries. Future
carrier hardening should consume CapabilityLease and ResourceScope before
dispatch and preserve explicit outcomes such as `subject_lacks_execute_authority`,
`resource_outside_scope` and carrier dispatch allowed.
