# ADR 0012: Naked Model Before Agent Framework

## Status

Accepted for SPINE.2. Roadmap numbering superseded by SPINE.3R data-plane
rebase.

## Context

YAI Core needs model/provider behavior evidence, but agent frameworks bring
their own planning loops, tool abstractions, memory assumptions and provider
semantics. If the first AI experiment starts at the agent layer, the core cannot
tell which behavior came from model output, projection shape, policy text,
agent orchestration or tool adapter behavior.

The core also must not treat a model provider as an internal subsystem. A model
is outside the core and becomes meaningful to YAI only when bound to a case and
represented through residue.

## Decision

The first AI behavior experiment is a naked model inside a case, not agent
framework integration.

```text
model is treated as a case-bound subject
model invocation/output enters through projection, claim, receipt and memory
agent frameworks are later ai-environment/adapters
```

L0 provider scouting can begin immediately outside the core. It can generate
notes and fixtures, but it is not canonical core validation.

## Consequences

SPINE.2 placed these model/provider experiments after filesystem refoundation.
SPINE.3R keeps the maturity order but moves implementation after the
NEW.22-NEW.30 data-plane foundation:

```text
first naked local model case experiment
then first core-owned model invocation through model carrier v0
then first agent-framework/tool-call experiment path
```

Model output is imported as a claim, observation or proposed operation before it
is trusted. Control evaluates proposed operations. Receipts record the import,
decision and behavior trace. Memory derives only from residue that has a
record, receipt or graph basis.

Policy is materialized twice for model work:

```text
cognitively through projection
operationally through gates, decision and carrier
```

## Non-Goals

SPINE.2 does not:

```text
implement model runner
implement provider registry
create model carrier
create policy packs
create ai-environment repo
touch old yai
touch interfaces
touch console
```

## Related Docs

```text
../engineering/model-provider-experiment-ladder.md
../engineering/naked-model-case-test-plan.md
../engineering/four-repo-roadmap.md
../architecture/04-subject-model.md
../architecture/10-projection-model.md
```
