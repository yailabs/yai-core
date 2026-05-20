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

## NEW.6 Candidate V0

NEW.6 adds `memory_candidate` as the first residue-derived memory layer.
It is a candidate record, not consolidated recall.

The candidate is derived from:

```text
store records
receipt refs
decision refs
subject refs
graph edge refs
receipt reconstruction chains
```

Initial candidate kinds:

```text
operational
decision
subject
error
recovery
```

Initial scope kinds:

```text
case
subject
local
```

Initial posture fields:

```text
freshness: fresh | stale | unknown
confidence: high | medium | low | unknown
```

The first smoke derives a decision memory candidate from a mutative filesystem
write that required review and was blocked. This proves traceable operational
experience without adding prompt memory, RAG, vector storage or model-owned
continuity.

## Reconcile Interaction

Memory candidates can themselves be checked by reconciliation. NEW.7 treats a
candidate with missing basis counts as divergence:

```text
memory_without_basis
```

This keeps memory as traceable continuity rather than hidden assertion.

## Projection Interaction

NEW.8 requires memory access to pass through controlled projection posture.
A memory candidate can be counted in a projection result, but projection must
report source memory counts, freshness and redaction posture. Model and agent
consumers receive limited summaries by default.

## Rust Direction

Advanced recall, ranking, consolidation, decay, pinning and privacy filtering are Rust engine candidates behind C FFI.

NEW.10 begins that direction only as residue consumption. Rust can count memory
candidate records through the engine path, but it does not consolidate, rank,
recall or widen memory scope.
