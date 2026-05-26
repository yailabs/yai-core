# Architecture Summary

## Overview

YAI is a local control runtime for case-bound AI operation. Its core concern is
not inference quality by itself, but the operational boundary around activity
that may affect files, services, providers, memory, records, and operator
decisions.

The architecture centers on cases, subjects, attempted operations, control
decisions, effects or observations, receipts, records, projections, and
operational memory.

## Case-Bound Operation

A case is the operational boundary. It scopes subjects, providers, policy
material, attempts, receipts, records, projections, and memory.

Case binding is what lets later inspection ask what happened, where it
happened, which boundary was touched, what was allowed or blocked, and what
evidence was produced.

## Subjects And Providers

A subject is an entity inside or around a case: file, process, repository,
service, model, operator, provider, tool, document, or external system.

A provider is a boundary that may supply model output, tool behavior, remote
service behavior, or mocked behavior. Providers participate in cases, but YAI
does not own inference and does not treat provider output as authority.

## Operations And Control Decisions

An attempted operation is proposed work against one or more subjects or
boundaries. It becomes operationally meaningful when it is bound to a case and
evaluated by control material.

Control decisions describe whether an attempt is allowed, blocked, constrained,
observed, imported, or paired with obligations. Policy text alone is not the
decision; the runtime decision is the material that can be recorded and
inspected.

## Effects, Observations, Receipts

An effect is execution or imported consequence at a boundary. An observation is
material learned without necessarily owning execution.

A receipt is structured evidence of what happened: the attempt, decision
posture, boundary, result, and affected subjects. Receipts are not generic log
lines; they are operational evidence connected to the case.

## Records And Operational Memory

Records are durable material used for reconstruction and inspection. They can
represent attempts, decisions, receipts, sources, policy material, and related
case residue.

Operational memory is derived from records, receipts, and related structure. It
is not just free text or model context; it is case-scoped memory from
operational residue.

## Projections

A projection is a controlled view served to a model, operator, API, audit,
debug, or external participant. Projection is not ownership of truth. It is a
scoped view over case material.

## Source / Runtime Boundary

The repository is split across public headers, C system boundary code, the Rust
operational data engine, local commands, tests, tools, and docs.

```text
include/    public and system ABI headers
system/     C daemon, control, carriers, host boundary, bridges, transitional shims
engine/     Rust operational data engine
cmd/        local binaries: yai and yaid
tests/      smoke and validation tests
docs/       curated architecture documentation
labs/       experiments, runbooks, outputs and reports
work/       spines, waves, inventories, agent notes and archives
```

## Current Implementation Caveats

Some C data-plane paths are transitional while Rust engine ownership is being
consolidated. Current command, source, testing and manual validation details
live in `work/spines/`, while lab execution material lives in `labs/`.

YAI is not production-ready unless explicitly stated, and provider/backend
mentions should not be read as tested provider breadth.

## Deeper References

- [Architecture notes](architecture/README.md)
- [Data runtime](architecture/data-runtime.md)
- [Control runtime](architecture/control-runtime.md)
- [Model runtime](architecture/model-runtime.md)
- [Glossary](glossary.md)
