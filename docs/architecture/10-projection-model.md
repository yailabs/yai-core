# Projection Model

Projection serves controlled read models over core residue.

Projection replaces the old final `views` wording in the new core. Console views are UX; core projections are data products.
Projection is not UI state and not source-of-truth state. It is the controlled
read model for model, agent, operator, API, audit and debug consumers.

## Projection Audiences

```text
model
agent
operator
audit
control
debug
API
embedded system
```

## Projection Properties

```text
source refs
freshness
provenance
redaction
audience
scope
policy constraints
receipt support
```

## Rule

Projection is not truth. Projection reads records, receipts, graph, memory and policy posture. It must not become UI state, dashboard authority or API truth.

## NEW.8 Projection Request / Result V0

NEW.8 adds explicit projection request and result records:

```text
projection_request
projection_result
```

A projection request declares:

```text
request id
case ref
consumer kind
projection kind
scope
summary
```

A projection result declares:

```text
projection id
request id
case ref
consumer kind
projection kind
source record count
source receipt count
source memory count
source divergence count
redaction posture
freshness
summary
```

Initial consumers:

```text
operator
model
agent
api
audit
debug
```

Initial projection kinds:

```text
operator_summary
model_context
agent_context
audit_packet
debug_dump
control_summary
memory_summary
```

Initial redaction posture:

```text
none
summary_only
refs_only
redacted
blocked
```

The model and agent defaults are `summary_only`. Debug defaults to `refs_only`.
This is not a full sensitive-data redaction engine; it is the first metadata
and posture boundary for controlled read models.

## Query Interaction

NEW.9 query can produce `query_result` residue. Projection may count those
results and expose them in read models, but projection does not execute query
planning or become query truth. Query remains a scan/filter boundary over store
records; projection remains a controlled view over residue.

## Rust Engine Interaction

NEW.10 allows the Rust engine to build projection summary JSON from existing
journal residue. SPINE.1 clarifies the target: projection materialization
belongs to the Rust operational data spine, exposed through C ABI and system
bridges without turning projection into UI state.

## NEW.13 Route

```text
lib/projection -> split: system/engine_bridge + engine/yai-engine/src/projection
../yai/src/substrate/views/* -> engine/projection concept evidence
../yai/src/case/surface/* -> projection/console future, not system UX
```

No projection implementation moves in NEW.13.
