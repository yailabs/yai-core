# ADR 0020: Control / Carrier Substrate

Status: Accepted.

## Context

The current runtime has a minimal filesystem carrier and growing record-plane
visibility. The old `yai-dev` material has richer runtime carrier, execution,
observation and decision concepts. Before adding more carriers or dispatch
runtime, `yai` needs a small vocabulary that prevents model/operator/API/tool
requests from being treated as direct execution.

## Decision

YAI introduces low-level C ABI primitives for carrier family, gate outcome,
dispatch status, receipt guarantee mode and host observation posture.

The boundary is explicit:

```text
observation is not enforcement
decision is not execution
carrier is not free tool invocation
receipt is not logging
```

`yai carrier families` exposes the vocabulary and current minimal/planned
status without executing carriers.

## Consequences

Future carrier waves must classify an operation candidate before execution.
They must distinguish control outcome, dispatch posture and receipt guarantee.
Observation may produce evidence or divergence, but it is not enforcement.

SPINE.33A adds no process carrier, network carrier, database carrier,
repository carrier, model carrier, dispatch queue, LMDB behavior, graph facts
or memory consolidation.
