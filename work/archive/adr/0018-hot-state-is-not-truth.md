# ADR 0018: Hot State Is Not Truth

Status: accepted.

## Decision

YAI treats hot state as the live operational cache of a case session.

Hot state is not truth. It may be rebuilt from durable residue and exists to
reduce world-to-view latency, track projection freshness and expose current
session diagnostics.

## Context

Before SPINE.23, too much operational state was inferred from the journal,
participant view or command-local state. That kept the journal in the active
path instead of limiting it to replay/audit.

## Consequences

The journal remains replay/audit.

Durable truth remains in residue:

```text
journal and records today
LMDB record plane later
Ladybug graph plane later
DuckDB fact plane later
receipts, memory, reconcile and rebuild evidence
```

The daemon owns local hot-state lifecycle. The CLI may read
`YAI_HOME/run/hot-state.json` for diagnostics.

Projection freshness and stale reason become explicit runtime state, but they
remain cache posture, not durable truth.
