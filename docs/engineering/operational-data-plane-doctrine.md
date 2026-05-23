# Operational Data Plane Doctrine

Status: SPINE.3R doctrine for future data-plane waves.

YAI Core has one residue model, but it does not have one physical data plane.
SPINE.3R separates hot state, replay, durable records, graph, facts, memory,
projection and reconcile so NEW.19 can align build and guard rules to the
actual target.

## Planes

| Plane | Role | Truth posture |
|---|---|---|
| hot plane | shared memory / hot state for active case, current projection frame, carrier locks, pending ops, open obligations and latest deltas | not source of truth |
| journal plane | append-only replay chronology | replay truth |
| record plane | LMDB or equivalent durable KV store for normalized records and lookup | durable record truth |
| graph plane | Ladybug persistent operational graph for causality and reconstruction | relationship truth |
| fact plane | DuckDB analytical facts for metrics, experiments, reports and model behavior analysis | derived analytical truth |
| memory plane | operational memory engine derived from records, receipts, graph, policy and divergences | consolidated continuity |
| projection plane | live, versioned, delta-aware view materialization | controlled cognitive view |
| reconcile plane | expected-vs-observed mismatch detection and recovery posture | mismatch truth |
| observability/evaluation plane | diagnostic, temporal, epistemic and behavioral facts over residue | quality posture, not truth |

## Backend Doctrine

```text
shared memory = hot state, not truth
journal = replay
LMDB = durable record plane
Ladybug = operational graph
DuckDB = derived facts / analytics
interaction thread = active conversational lane, journal-backed now and hot/record-backed later
Rust engine = data-plane materialization and query
C system = host boundary, daemon, carrier, control shell, FFI bridge
```

SPINE.3R does not implement shared memory, LMDB, Ladybug or DuckDB. Those
backends are planned-not-created until their dedicated waves.

## Wave Corrections

```text
NEW.22 Hot state / shared memory plane v0
NEW.23 LMDB record backend v0
NEW.24 Journal replay to LMDB v0
NEW.25 Ladybug graph backend v0
NEW.26 DuckDB fact backend v0
NEW.27 Projection delta / live view v0
NEW.28 Memory consolidation over record + graph + facts v0
NEW.29 Reconcile over hot/store/graph consistency v0
NEW.30 Data plane milestone freeze
```

Ingest, model/provider, policy-pack and agent-framework work continues only
after the data-plane foundation is explicit.

## SPINE.4 Observability Overlay

The observability/evaluation plane reads across the operational data planes:

```text
hot plane      -> active case version, latest delta, freshness, divergence flags
journal plane  -> trace records, timing samples, diagnostic records
record plane   -> trace lookup and case-view version lookup
graph plane    -> causal reconstruction and path completeness
fact plane     -> aggregate metrics and model behavior facts
memory plane   -> memory basis quality and contradiction rate
projection     -> freshness, delta, redaction and authority alignment
reconcile      -> divergence exposure and mismatch posture
```

SPINE.4 does not implement observability records, DuckDB analytics, debug
commands or rebuild tooling. It makes the measurement surface canonical before
NEW.19 guard realignment.
