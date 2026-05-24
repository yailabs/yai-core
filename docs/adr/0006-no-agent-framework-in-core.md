# ADR 0006: No Agent Framework In Core

## Status

Accepted for DOC.NEW.1.

## Context

YAI must integrate with agents without becoming an agent framework. Planning,
roles, prompt orchestration and context-pack assembly are useful, but they are
not the local operational control core.

## Decision

YAI does not implement an agent framework.

## Consequences

Agents can be subjects, actors, sources, targets or external systems. Agent labs
and framework adapters belong outside the core, mainly in `yai-dev` or
integrations. Core owns attempts, decisions, receipts, memory and projections
around agents.

## Non-goals

Do not make planning, roles, prompt orchestration or agent context packs core
roots. Do not sell YAI as another assistant.

## Supersedes

Any interpretation of `src/agents` as a future `yai` root.

## Related docs

```text
../product/positioning.md
../architecture/04-subject-model.md
../engineering/extraction-plan.md
```
