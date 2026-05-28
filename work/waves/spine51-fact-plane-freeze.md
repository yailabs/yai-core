# SPINE.51 - Fact Plane Freeze

SPINE.51 freezes the first DuckDB Fact Plane milestone after schema, init,
extraction, summary and report.

Fact Plane v1 is a derived analytical layer over existing YAI records,
relations, projections, receipts, decisions, carriers, memory candidates and
runtime diagnostics. Facts are analytical, temporal, provenance-bearing
assertions. Facts are not record truth, graph truth, memory, policy authority,
reconcile action or model authority.

Frozen command surface:

```bash
yai facts status
yai facts schema
yai facts init
yai facts extract --case <case_ref> --kind core
yai facts extract --case <case_ref> --kind behavior
yai facts extract --case <case_ref> --kind operational
yai facts extract --case <case_ref> --kind all
yai facts summary --case <case_ref>
yai facts report --case <case_ref>
yai facts report --case <case_ref> --section receipts
yai facts report --case <case_ref> --section decisions
yai facts report --case <case_ref> --section projections
yai facts report --case <case_ref> --section policy
yai facts report --case <case_ref> --section carriers
yai facts report --case <case_ref> --section divergence
yai facts report --case <case_ref> --section memory
yai facts report --case <case_ref> --section model
```

Frozen DuckDB schema posture:

```text
schema: yai.fact.v1
backend: DuckDB
bitemporal fields: transaction_time, valid_time_start, valid_time_end, known_at
revision fields: revision_of, superseded_by, retracted_by
status fields: current, superseded, retracted, stale, contested,
  historical_only, branch_only, counterfactual, needs_review, unknown
```

Frozen fact classes:

```text
fact_receipt
fact_decision
fact_projection
fact_carrier_outcome
fact_divergence
fact_model_behavior
fact_policy_outcome
fact_memory_quality
```

Expected filesystem-loop summary after the installed matrix:

```text
facts_summary:
case_ref: case:new12-filesystem
fact_receipt: 3
fact_decision: 2
fact_projection: 3
fact_carrier_outcome: 3
fact_divergence: 0
fact_model_behavior: 0
fact_policy_outcome: 7
fact_memory_quality: 1
facts_total: 19
facts_are_truth: false
memory_is_truth: false
```

Zero divergence/model facts are valid when source records are absent. Report
sections must surface:

```text
divergence:
  total: 0
  status: none_observed

model:
  total: 0
  status: no_model_records

memory:
  memory_is_truth: false
```

Extraction remains idempotent extraction. A second `yai facts extract --case
case:new12-filesystem --kind all` must write zero new facts and report
duplicates for the existing derived rows.

Validation:

```bash
make check-fact-plane-freeze
make smoke-spine51
```

Smoke labels:

```text
fact_freeze:status ok
fact_freeze:schema ok
fact_freeze:init ok
fact_freeze:extract_core ok
fact_freeze:extract_behavior ok
fact_freeze:extract_operational ok
fact_freeze:extract_all ok
fact_freeze:idempotent ok
fact_freeze:summary ok
fact_freeze:report ok
fact_freeze:divergence none_observed ok
fact_freeze:model no_model_records ok
fact_freeze:not_truth ok
fact_freeze:memory_not_truth ok
```

Non-goals:

```text
No new fact tables.
No new fact extraction classes.
No fact revision implementation.
No retrieval quality extraction.
No provider runtime facts.
No report file artifacts.
No chart generation.
No audit packet.
No reconcile action.
No memory consolidation.
No HNSW.
No Context Compiler.
No CLORI.
No yai-dev mutation.
```
