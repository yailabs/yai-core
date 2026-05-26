# ADR 0021: Operation Dispatch Multiplex

Status: accepted
Owner: control/carrier substrate
Purpose: Decide how operation attempts are routed before carrier execution.
Not source of truth for: process carrier v0 or host observation probes

## Decision

Operation dispatch is separate from decision and execution. YAI uses carrier
lanes, not a single global queue. Dispatch plans are inspectable and can be
generated without executing effects.

## Context

SPINE.33A defined the vocabulary for carrier families, gate outcomes, dispatch
status, receipt guarantees and host observation posture. SPINE.33B adds route
planning so a carrier family can map to a lane without inventing ad hoc command
paths for future process, network or repository effects.

## Consequences

`filesystem` routes to `filesystem_lane` with `active_minimal` lane status.
Planned families route to planned lanes. Unknown families route to
`unknown_lane` with `dispatch_status: not_supported`.

All SPINE.33B plans report:

```text
execution_performed: false
```

Future carriers must preserve the distinction between decision, dispatch,
execution, receipt and observation.
