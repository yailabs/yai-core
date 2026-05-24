# Operational Observability And Evaluation

Status: SPINE.4 canonical observability/evaluation doctrine.

YAI does not only control actions. It measures whether the case remains
knowable enough for controlled action.

The Operational Observability & Evaluation Plane measures the quality of the
relationship between the live case and every subject that acts within it. It
tracks whether operational residue becomes visible, causal, provenanced,
complete, coherent, replayable and useful fast enough for models, agents,
operators and carriers to behave correctly.

It does not replace the case. It evaluates whether the case can be trusted as a
living operational substrate.

## Case View Quality

The canonical quality vector is `Case View Quality`, abbreviated `CVQ`.

CVQ fields:

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

CVQ is not truth. It is derived diagnostic posture over residue, projection,
memory, reconcile, graph and timing facts.

## Fact Classes

Temporal facts:

```text
receipt_to_record_ms
record_to_projection_ms
projection_to_model_ms
model_to_interpretation_ms
interpretation_to_attempt_ms
attempt_to_decision_ms
decision_to_receipt_ms
receipt_to_memory_ms
receipt_to_next_projection_ms
```

Epistemic facts:

```text
projection_freshness
memory_basis_quality
graph_chain_completeness
provenance_sufficiency
case_context_consistency
record_replayability
```

Behavioral facts:

```text
model_refusal
model_authority_overclaim
unsafe_attempt
unsupported_claim
review_request
policy_compliance
memory_use
divergence_trigger
```

## Relationship To Data Planes

| Plane | Observability input |
|---|---|
| hot | active case version, latest delta, projection freshness, critical divergence flags and pending obligations |
| journal | trace records, timing samples and diagnostic records |
| record | trace lookup, diagnostic state lookup and case-view version lookup |
| graph | causal reconstruction, path completeness and orphan edge detection |
| fact | aggregate metrics, model behavior facts, projection latency, policy outcomes and memory quality facts |
| memory | basis quality, reuse quality and contradiction rate |
| projection | freshness, delta, redaction, provenance and authority alignment |
| reconcile | divergence exposure and mismatch posture |

## Future Record Kinds

SPINE.4 documents these record kinds but does not implement them:

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

## Future Commands

SPINE.4 documents these commands but does not implement them:

```text
yai debug trace
yai debug freshness
yai debug rebuild
yai debug model-behavior
yai debug memory-quality
yai debug projection-lag
```

## Doctrine

Projection is not only a summary. Projection is a live, versioned,
delta-aware cognitive view over the operational data planes of a case.

The model does not access the case. The model receives a controlled live view
of the case.

YAI owns the freshness and quality of the case view. The model owns its
inference behavior and inference latency.
