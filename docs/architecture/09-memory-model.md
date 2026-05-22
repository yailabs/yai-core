# Memory Model

Memory is scoped consolidated operational continuity.

Memory is not prompt history, chat transcript, hidden model state, generic
knowledge, record truth or UI state. YAI does not sell memory as chat history.
YAI memory is operational memory: decisions, receipts, subject state, policy,
graph paths and divergences.

Comparison:

| Term | Meaning |
|---|---|
| RAG | retrieves documents or knowledge |
| Agent memory | remembers textual or task experience |
| YAI memory | remembers decisions, receipts, subject state, policy, graph paths and divergences |

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
case-bound
subject-aware
policy-aware
receipt-backed
graph-derived
scoped
freshness-aware
projection-controlled
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

Advanced recall, ranking, consolidation, decay, pinning and privacy filtering
belong to the Rust operational data spine behind C FFI.

NEW.10 begins that direction only as residue consumption. Rust can count memory
candidate records through the engine path, but it does not consolidate, rank,
recall or widen memory scope.

## NEW.13 Route

```text
lib/memory -> split: system/engine_bridge + engine/yai-engine/src/memory
../yai/src/substrate/memory/* -> concept evidence only
../yai/src/lineage/* -> graph-derived memory evidence only
../yai/src/analytics/* -> memory/reconcile/index evidence or ai-environment
```

No memory implementation moves in NEW.13.

NEW.14 moved the Rust engine crate to `engine/yai-engine`, making that the
physical owner for future memory mechanics. C memory code remains under
`lib/memory` until the NEW.18 bridge split.
