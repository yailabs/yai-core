# Memory Model

Memory is scoped consolidated operational continuity.

Memory is not prompt history, chat transcript, hidden model state, generic knowledge, record truth or UI state.

## Memory Kinds

```text
episodic memory
decision memory
policy memory
subject memory
model behavior memory
actor behavior memory
error memory
recovery memory
```

## Required Properties

```text
scope
origin
consolidation reason
linked records / receipts / graph edges
privacy boundary
retention or forgetting posture
traceability
projection permissions
```

## Consolidation Rule

Observed material does not become memory automatically. Memory candidates must pass scope, origin, privacy, evidence and trace requirements.

## Rust Direction

Advanced recall, ranking, consolidation, decay, pinning and privacy filtering are Rust engine candidates behind C FFI.
