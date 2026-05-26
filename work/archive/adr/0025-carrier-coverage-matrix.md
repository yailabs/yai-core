# ADR 0025: Carrier Coverage Matrix

Status: accepted

## Decision

YAI tracks carrier coverage by family, mode and outcome posture.

Every effect-capable family must be visible even if execution is not yet
implemented.

## Consequences

- Filesystem and process remain the only active controlled carrier examples.
- Network, database, repository, service, endpoint, socket, listener and model
  provider surfaces are visible without fake execution.
- Future carrier waves must update the matrix before adding behavior.
