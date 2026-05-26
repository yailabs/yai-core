# ADR 0016: Operational Observability Evaluation Plane

## Status

Accepted for SPINE.4.

## Context

NEW.18B makes the active case a loaded `case_session` and `case_context`
rather than only refs and journal paths. SPINE.3R makes projection live,
versioned and delta-aware over operational data planes.

The next spine needs to say how YAI knows whether that live view is usable
enough for controlled action.

## Decision

YAI treats Operational Observability & Evaluation as a cross-cutting
plane.

It derives diagnostic, temporal, epistemic and behavioral facts from
operational residue. It does not own truth. It measures whether the live case
remains fresh, causal, provenanced, coherent, complete, replayable and useful
enough for controlled model, operator and carrier behavior.

The canonical quality vector is `Case View Quality` (`CVQ`):

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

## Consequences

Every future implementation wave must declare its observability and freshness
impact. Projection, memory, reconcile, graph, fact and hot-state work must be
designed so trace, provenance, rebuild, model behavior and memory quality can
be measured later.

## Non-goals

```text
do not implement observability records in SPINE.4
do not implement DuckDB, LMDB, Ladybug or shared memory in SPINE.4
do not add debug commands in SPINE.4
do not let observability replace residue truth
do not let model latency be confused with case-view freshness
```

## Related docs

```text
../architecture/14-operational-observability-evaluation.md
../engineering/observability-evaluation-plane.md
../engineering/case-view-quality.md
../engineering/operational-data-plane-doctrine.md
../engineering/live-projection-model.md
```
