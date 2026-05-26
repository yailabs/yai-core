# ADR 0003: Daemon Over Runtime

## Status

Accepted for DOC.NEW.1.

## Context

The old `runtime` root accumulated activation, lifecycle, machine, connections,
execution, provider, operator and session concerns. That made `runtime` a
catch-all rather than a machine primitive.

## Decision

The future resident process is `yaid` under `daemon`, not a `runtime` root.

## Consequences

Daemon owns boot, config, loop, local IPC, event stream, dispatch, status,
supervision and host integration. Execution belongs to `effect`. Policy belongs
to `control`. Technical commands belong to `ctl`.

## Non-goals

Do not use `runtime` as a catch-all for execution, control, provider, operator
UX and sessions. Do not make daemon own product UX.

## Supersedes

The old `src/runtime` root as a future core root.

## Related docs

```text
../architecture/12-daemon-and-ctl.md
../engineering/target-layout.md
../engineering/ctl-console-split.md
```
