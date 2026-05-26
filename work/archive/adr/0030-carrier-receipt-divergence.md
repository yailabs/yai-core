# ADR 0030: Carrier Receipt Divergence

Status: accepted

## Context

SPINE.33H made carrier outcome posture representable across active, skeleton
and unsupported carrier families. The next gap is consistency: an outcome
posture only matters if it can be reconciled against decision, dispatch,
receipt and observation.

## Decision

YAI treats carrier receipt consistency as a reconciled relationship between
decision, dispatch, outcome, receipt and observation.

Inconsistency becomes divergence posture, not silent repair.

## Consequences

- `yai carrier reconcile-outcome` exposes diagnostic consistency scenarios.
- Clean scenarios report `result: consistent`.
- Inconsistent scenarios report `divergence_candidate` and severity.
- SPINE.33I adds no new carrier execution and no provider/runtime behavior.
