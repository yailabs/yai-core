# NEW.6 Operational Memory Candidate

NEW.6 introduces the first memory candidate layer. It is operational memory
derived from core residue, not prompt memory, chatbot memory, RAG memory, vector
memory or hidden model state.

## Old-Yai Audit

Read-only audit scope:

```text
../yai/src/substrate/memory
../yai/src/lineage/episodic_summary.c
../yai/src/lineage/semantic_summary.c
../yai/src/analytics/signals
../yai/src/agents/grounding/memory_strategy.c
../yai/src/agents/grounding/retrieval_strategy.c
../yai/src/agents/grounding/query_strategy.c
```

Extracted concepts:

```text
memory candidates must be scoped
memory candidates must link back to records, receipts and graph paths
freshness and confidence are non-authoritative posture fields
signals may inform memory posture but do not own truth
agent grounding strategies belong outside core unless reduced to primitive refs
```

No old source was copied.

## Contract

Memory candidate fields:

```text
memory_id
case_ref
memory_kind
scope_kind
basis_record_count
basis_receipt_count
basis_edge_count
subject_ref
decision_ref
receipt_ref
freshness
confidence
summary
```

Initial memory kinds:

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

Initial freshness:

```text
fresh
stale
unknown
```

Initial confidence:

```text
high
medium
low
unknown
```

## Derivation V0

The first derivation path starts from an existing receipt reconstruction:

```text
journal records
receipt refs
decision refs
subject refs
graph edge refs
reconstruction chain
```

It emits a `memory_candidate` store record with a compact summary such as:

```text
memory:decision scope:subject freshness:fresh confidence:high basis_records:10 basis_receipts:1 basis_edges:5
```

This is a candidate, not consolidated memory. It has enough provenance for a
projection and later policy/reconcile/memory consolidation work.

## Smoke

`tests/smoke/operational-memory/test_operational_memory.c` proves:

```text
open case
bind subject
submit mutative fs.write attempt
emit require_review decision
emit blocked receipt
append graph edges
reconstruct receipt chain
derive memory candidate
persist memory_candidate record
reload journal
build memory projection
```

Debug command:

```text
crates/target/debug/yaictl memory summary --journal build/tmp/new6/operational-memory-<pid>/journal.jsonl
```

## Out Of Scope

```text
no prompt memory
no vector store
no RAG implementation
no model memory provider
no memory consolidation engine
no cross-case recall
no graph database
no Rust production backend switch
```
