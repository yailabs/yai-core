Historical/superseded engineering record. Not an active source of truth.

# Observability Evaluation Plane

Status: SPINE.5 OBSERVABILITY spine doctrine, based on SPINE.4.

The Operational Observability & Evaluation Plane is a cross-cutting plane over
case world, case context, projection, memory, reconcile and the operational
data planes. It derives diagnostic, temporal, epistemic and behavioral facts
from operational residue.

It does not own truth. It measures whether the live case remains fresh, causal,
provenanced, coherent, complete, replayable and useful enough for controlled
model, operator and carrier behavior.

## Responsibilities

```text
measure case-view freshness
measure causal completeness
measure provenance sufficiency
measure projection consistency
measure authority alignment
measure memory basis quality
measure divergence exposure
measure delta accuracy
measure cost
```

## Plane Inputs

| Source | Measurement posture |
|---|---|
| `case_session` | active lifecycle and case version |
| `case_context` | live operating surface consistency |
| `case_world` | attachment, binding, authority and projection posture |
| `journal` | replay order and timing evidence |
| `record plane` | normalized residue lookup |
| `graph plane` | causality and reconstructability |
| `fact plane` | aggregate behavior and latency facts |
| `memory plane` | basis quality and reuse quality |
| `projection plane` | freshness, delta and redaction posture |
| `reconcile plane` | mismatch exposure |

## Future Records

The following are planned-not-created until the implementation waves:

```text
trace
trace_span
timing_sample
freshness_state
projection_delta
rebuild_report
diagnostic_signal
analytics_fact
model_behavior_fact
memory_quality_fact
projection_freshness_fact
```

## Future Debug Commands

The following commands are documented as future technical command surfaces only:

```text
yai debug trace
yai debug freshness
yai debug rebuild
yai debug model-behavior
yai debug memory-quality
yai debug projection-lag
```

They must not be implemented by SPINE.4.

## Future Wave Template

Every future implementation wave must include an `Observability / freshness
impact` section and answer:

```text
Does this wave create new residue?
Does this wave affect hot state?
Does this wave invalidate projection?
Does this wave create or require a delta?
Does this wave affect model-visible freshness?
Does this wave add trace/span/timing needs?
Does this wave affect provenance/completeness/reconstructability?
Does this wave produce behavioral facts or memory quality facts?
```

NEW.18C observability impact:

```text
thread switch invalidates participant_view_frame
new interaction_turn creates a participant-view delta
transcript mode changes audit completeness, not authority
projection freshness must be checked before model prompt
```

SPINE.5 makes this template mandatory for every future implementation wave and
tracks the work under the `OBSERVABILITY` macro-spine. The full delivery
template is maintained in `wave-delivery-template.md`.
