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
