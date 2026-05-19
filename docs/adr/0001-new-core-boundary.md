# ADR 0001: New Core Boundary

## Status

Accepted for DOC.NEW.1.

## Context

The current repository contains rich historical roots such as `agents`,
`capabilities`, `decision`, `models`, `orchestrator`, `runtime` and `substrate`.
Those roots contain useful concepts, but recreating them as the future core
would preserve the old architectural confusion.

## Decision

YAI Core is a local AI operational control core, not a cleaned-up copy of the
current `src/` tree.

Future roots:

```text
base ingest subject case op control effect store index graph memory projection reconcile daemon ctl
```

## Consequences

Old roots are concept sources, not migration targets. The future core starts
from the machine spine and NEW.1 loop. Extraction must be inventory-driven and
primitive-driven.

## Non-goals

Do not recreate `agents`, `capabilities`, `decision`, `models`, `orchestrator`,
`runtime` or `substrate` as core roots.

## Supersedes

Earlier folder-preserving interpretations of the core spine.

## Related docs

```text
../architecture/00-spine.md
../engineering/target-layout.md
../engineering/extraction-plan.md
```
