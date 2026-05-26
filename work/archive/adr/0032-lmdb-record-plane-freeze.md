# ADR 0032: LMDB Record Plane Freeze

Status: accepted

## Context

SPINE.29 defined LMDB as durable indexed record lookup. SPINE.30 added the
write path. SPINE.31 added id/case/kind reads. SPINE.32 added subject and
receipt indexes. SPINE.33 froze CLI/manual validation.

The carrier/control substrate now has outcome, receipt and divergence posture.
The record plane needs a freeze before journal replay and graph persistence
waves build on it.

## Decision

SPINE.34 LMDB Record Plane Freeze accepts the current `yai.record.v1` LMDB
surface as stable enough for the next phase.

LMDB is durable indexed record lookup. journal remains replay/audit. There is
no journal fallback for record reads.

Frozen indexes:

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
schema_meta
```

The record plane must be able to store and query control/carrier residue,
including `carrier_request`, `effect_receipt`, `filesystem_receipt` and
`divergence` records, without adding new carrier execution behavior.

## Consequences

- SPINE.35+ journal replay work must target the frozen `yai.record.v1` lookup
  and index posture.
- Missing or unavailable LMDB must remain explicit status, not journal fallback.
- Graph, facts, memory, RuntimeGraph, HNSW and projection remain separate
  planes.

## Non-Goals

No replay implementation, graph persistence, DuckDB facts, memory
consolidation, RuntimeGraph, HNSW, projection deltas or carrier execution is
added by this decision.
