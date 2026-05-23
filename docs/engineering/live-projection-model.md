# Live Projection Model

Status: SPINE.3R projection doctrine.

Projection is not removed. Static projection is superseded by live, versioned,
delta-aware projection.

Canonical definition:

```text
A projection is not a summary.
A projection is a versioned cognitive view over the operational data planes of a case.
```

Models do not access the case directly. Models receive controlled live
projections of the case.

## Required Fields

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

## Source Planes

Live projection reads from:

```text
hot plane
journal plane
record plane
graph plane
fact plane
memory plane
reconcile plane
```

NEW.18B adds the immediate runtime source:

```text
case_session -> case_context -> live projection
```

NEW.18C adds the model-visible assembly path:

```text
case_session -> active_interaction_thread -> participant_view_frame
```

The frame combines current case projection with selected thread turns. It does
not treat the journal as chat memory.

The current implementation is still journal-backed. The journal stores refs;
the runtime operates on case context derived from journal records and
case-world material.

Projection materialization belongs to the Rust engine. The C system boundary
can request, carry, inspect or expose projections through public ABI and daemon
paths, but it must not become the long-term projection engine.

## Model Boundary

Model-facing projection must preserve:

```text
case boundary
authority scope
redaction posture
policy posture
source refs
freshness
delta lineage
```

Model output is a claim, proposal or `model_interpretation` record. It is not
case truth, policy truth, receipt truth or execution authority.

## Case View Quality

SPINE.4 adds Case View Quality, `CVQ`, as the quality vector for live
projection:

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

Projection must provide enough source refs, case version, delta lineage,
redaction posture and authority posture for CVQ to be evaluated. The model does
not own CVQ; YAI owns the freshness and quality of the case view.
