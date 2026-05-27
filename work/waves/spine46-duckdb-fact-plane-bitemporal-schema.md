# SPINE.46 - DuckDB Fact Plane Doctrine + Bitemporal Schema

Status: complete.

## Purpose

Introduce DuckDB as YAI's analytical fact plane without extracting facts yet.

A fact is not just a row. A fact is a temporally scoped,
provenance-bearing assertion.

## Doctrine

Journal records chronology. LMDB stores record truth. Graph persistence stores
relation truth. RuntimeGraph computes over active case relations. DuckDB stores
analytical, temporal, provenance-bearing facts derived from records, graph
relations, receipts, decisions, projections, carriers, model behavior, replay
reports, runtime graph diagnostics, retrieval and memory.

Facts are not truth. Facts can explain, score, measure, compare, filter and
report. Facts cannot authorize, execute, approve, deny, mutate operational
truth or replace evidence.

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

## Schema

Schema identity:

```text
yai.fact.v1
```

DuckDB location:

```text
YAI_HOME/store/facts/yai-facts.duckdb
```

Common bitemporal/revision posture:

```text
transaction_time
valid_time_start
valid_time_end
known_at
status
revision_of
superseded_by
retracted_by
```

Initial tables:

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

## Command Surface

```bash
yai facts status
yai facts schema
yai facts init
```

`yai facts init` creates the DuckDB file and the twelve schema tables. No fact
extraction happens in this wave; `facts_extracted: 0` is the required output
posture. Fact extraction begins in SPINE.47.

## Validation

```bash
make check-duckdb-fact-plane
make smoke-spine46
```

Expected smoke labels:

```text
fact_plane:status ok
fact_plane:schema ok
fact_plane:bitemporal ok
fact_plane:not_truth ok
fact_plane:init ok
fact_plane:tables ok
fact_plane:no_extraction confirmed
```

## Non-Goals

No fact extraction. No report generation. No model behavior scoring. No memory
quality computation. No retrieval quality computation. No RuntimeGraph changes.
No HNSW. No Context Compiler. No carrier behavior changes.
