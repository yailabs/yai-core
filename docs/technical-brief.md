# Technical Brief

## Summary

YAI is a local control runtime for case-bound AI operation. It gives model,
provider, tool, operator, and system activity an operational boundary: what
case it belongs to, what subjects it affects, what policy applies, what was
allowed or blocked, what receipt was produced, and what record, projection, or
memory can be derived from the result.

## Problem Boundary

Model output can cross into files, services, provider calls, memory, operator
decisions, and workflow state. Once that happens, generation quality is not the
only technical question. The runtime also needs a way to inspect authority,
scope, effects, evidence, and recovery posture.

YAI addresses that boundary locally. It does not try to own every surrounding
system; it binds activity into cases and records the operational residue needed
to inspect what happened.

## Core Abstraction: Case

A case is the operational frame. It gives subjects, providers, attempts,
control decisions, receipts, records, projections, and memory a shared scope.

Agent-like activity may appear inside or around a case, but agent activity is
not the foundation. The foundation is case-bound operational control.

## Runtime Model

```text
case -> subject/provider -> attempted operation -> control decision -> effect/observation -> receipt -> record -> projection/memory
```

The model is intentionally small: bind the work to a case, identify the
subject or provider boundary, evaluate control material, observe or execute at
a boundary, write a receipt, update records, and derive the projection or
memory that later participants may see.

## Provider And Model Boundary

YAI does not own inference. Provider and model output is candidate material,
not authority.

Providers may be local, remote, specialized, custom, or mocked. A provider can
participate in a case, but provider breadth should not be inferred from a name
appearing in documentation. No DS4 integration or tested support is claimed by
this brief.

## Records, Receipts, Projections, Memory

Receipts and records are first-class runtime material. They preserve what was
allowed, blocked, observed, imported, or produced at a boundary.

Projection and memory are derived from operational residue. They are scoped
views over case material, not replacements for receipts and records.

## Current Implementation Surface

The repository currently contains:

- `cmd/`: local command and daemon entrypoints such as `yai` and `yaid`.
- `system/`: C system boundary for daemon, control, carriers, bridge code, and
  transitional shims.
- `engine/`: Rust operational data engine under consolidation.
- `include/`: public and system ABI headers.
- `tests/`: smoke and validation coverage.
- `docs/`: public entrypoints plus current engineering references.

Some C data-plane paths are transitional while Rust engine ownership is being
consolidated.

## What Can Be Evaluated Today

The repository can be inspected, built, and checked locally through the current
Makefile entrypoints. Start with:

```sh
make info
make check
```

The current command, source, testing, and manual validation details live in:

- [Command surface](engineering/command-surface.md)
- [Source surface](engineering/source-surface.md)
- [Testing](engineering/testing.md)
- [Filesystem loop manual](manuals/manual-filesystem-loop-validation.md)

## What Is Not Claimed

YAI is not an inference engine, model provider, chatbot framework, generic
agent framework, workflow builder, cloud platform, TUI/client application,
generic policy engine, or generic audit logger.

YAI is not production-ready unless explicitly stated. The command/test surface
is still stabilizing, public docs are still being split, and provider/backend
mentions should not be read as tested breadth.
