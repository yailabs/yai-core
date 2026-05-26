# ADR 0007: No Workflow Engine In Core

## Status

Accepted for DOC.NEW.1.

## Context

Workflow systems can produce important operational effects, but making YAI
own workflow execution would turn the core into an orchestrator. The core must
control and reconstruct attempts around workflows without becoming the workflow
engine.

## Decision

YAI does not implement a workflow engine.

## Consequences

Workflow systems can be subjects. Workflow events can be ingested. Procedure
records can belong to cases. Reconciliation can handle workflow divergence. The
core can interpose, observe or import receipts depending on integration mode.

## Non-goals

Do not recreate `orchestrator/workflow` as a future core engine. Do not make
case procedure records a workflow runtime.

## Supersedes

Any interpretation of `src/orchestrator/workflow` as a future core engine.

## Related docs

```text
../architecture/02-integration-modes.md
../architecture/03-case-domain.md
../product/wedges.md
```
