# ADR 0015: Projection As Live Cognitive View

## Status

Accepted for SPINE.3R.

## Context

Projection was already defined as controlled read model, but some older wording
can still read as static summary material. Case-bound model operation requires
projection to carry version, freshness, redaction, authority and delta posture.

## Decision

A projection is not a summary. A projection is a versioned cognitive view over
the operational data planes of a case.

Static projection is superseded by live, versioned, delta-aware projection.
Models do not access the case directly. Models receive controlled live
projections of the case.

## Consequences

Projection remains non-authoritative. It serves controlled views over residue,
policy, authority scope, receipts, memory, graph, facts and divergences. Model
output remains claim, proposal or `model_interpretation` residue until checked
against decisions, receipts, graph, memory and reconcile records.
