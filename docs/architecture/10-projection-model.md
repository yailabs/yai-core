# Projection Model

Projection serves live controlled cognitive views over core residue.

Projection replaces the old final `views` wording in the new core. Console
views are UX; core projections are data products. Projection is not UI state
and not source-of-truth state. It is the live, versioned cognitive view for
model, agent, operator, API, audit and debug consumers.

Canonical SPINE.3R definition:

```text
A projection is not a summary.
A projection is a versioned cognitive view over the operational data planes of a case.
```

Models do not access the case directly. Models receive controlled live
projections of the case. NEW.18B clarifies that projection reads from the
materialized case world/context, not directly from loose `--journal`, `--case`
and `--subject` arguments.

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
freshness_policy
freshness_source
stale_reason
provenance
redaction
audience
scope
policy constraints
authority scope refs
case version
delta lineage
receipt support
case view quality
```

## Rule

Projection is not truth. Projection reads case-world material, records,
receipts, graph, facts, memory, reconcile and policy posture. It must not
become UI state, dashboard authority or API truth.

SPINE.4 defines Case View Quality (`CVQ`) as the canonical vector for measuring
whether a projection is useful enough for controlled behavior:

```text
freshness
causal_completeness
provenance_sufficiency
projection_consistency
authority_alignment
memory_basis_quality
divergence_exposure
delta_accuracy
cost
```

Projection must carry enough refs, version posture and freshness posture for
CVQ to be computed. CVQ is diagnostic posture, not case truth.

## Freshness Policy

SPINE.26 makes projection freshness consumer-aware.

```text
usable
refresh_recommended
refresh_required
blocked_for_model
unknown
```

Model and agent consumers have stricter requirements than operator, audit and
debug consumers. A stale model-visible projection caused by a new receipt,
decision or memory requires refresh. Staleness caused by authority scope,
divergence, thread change, manual refresh requirement or unknown reason blocks
model use in policy terms. SPINE.26 v0 warns on the prompt path; later waves may
hard-block when refresh is available as a complete workflow.

## Observability Boundary

The model does not access the case. The model receives a controlled live view
of the case. YAI owns the freshness and quality of that view. The model owns
its inference behavior and inference latency.

NEW.18C adds the interaction-thread boundary. Model context is the current
projection plus selected thread turns and authorized memory/delta material. It
is not the full journal, all previous `model_interpretation` records or raw
transcript.

Future projection work must make these facts measurable:

```text
record_to_projection_ms
projection_to_model_ms
projection_freshness
projection_consistency
delta_accuracy
authority_alignment
model_authority_overclaim
unsupported_claim
```

SPINE.4 documents future commands such as `yai debug freshness` and
`yai debug projection-lag`; it does not implement them.

## Live Projection Fields

```text
projection_id
case_version
consumer_kind
projection_kind
source_record_refs
source_receipt_refs
source_memory_refs
source_divergence_refs
policy_refs
authority_scope_refs
delta_since_projection_id
freshness
redaction_posture
summary
```

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

## Model Projection Doctrine

For model/provider experiments, policy is materialized twice:

```text
cognitively through projection
operationally through gates, decision and carrier
```

Model projection can include:

```text
allowed actions
forbidden actions
review requirements
evidence requirements
policy summaries
memory candidates
redaction posture
pack-derived projection rules
pack-derived output schemas
pack-derived authority wording
```

Projection is not enforcement. It shapes what the model sees and how the task
is framed. Control still evaluates proposed operations, obligations, evidence
requirements and receipt requirements. Model output is claim, proposal or
`model_interpretation` residue, not authoritative case truth.

`case_context` is not projection truth. It is the active runtime
materialization from which projection can be derived. Durable truth remains in
refs, records, receipts, graph, memory and divergence.

L0 provider scouting can start immediately outside the core and can produce
projection fixtures. SPINE.3R rebases implementation order so canonical
model/provider work follows the SPINE.23-SPINE.80 data-plane foundation.
Agent-framework projection work follows after naked model behavior is
measurable.

## Query Interaction

NEW.9 query can produce `query_result` residue. Projection may count those
results and expose them in read models, but projection does not execute query
planning or become query truth. Query remains a scan/filter boundary over store
records; projection remains a controlled view over residue.

## Rust Engine Interaction

NEW.10 allows the Rust engine to build projection summary JSON from existing
journal residue. SPINE.3R clarifies the target: live projection materialization
belongs to the Rust operational data spine, exposed through C ABI and system
bridges without turning projection into UI state.

## NEW.13 Route

```text
system/projection -> split: system/engine_bridge + engine/yai-engine/src/projection
../yai/src/substrate/views/* -> engine/projection concept evidence
../yai/src/case/surface/* -> projection/console future, not system UX
```

No projection implementation moves in NEW.13.

NEW.14 moved the Rust engine crate to `engine/yai-engine`, making that the
physical owner for future projection mechanics. C projection code remains under
`system/projection` as `keep_temporarily` after the NEW.18 bridge split.

SPINE.21 defines packs as case materialization units. Pack projection material
may shape model/operator/audit views only after it has been inspected,
validated and bound to case-world residue. Projection still does not enforce
policy or create case truth.

SPINE.28 is the planned projection delta / live view wave.
