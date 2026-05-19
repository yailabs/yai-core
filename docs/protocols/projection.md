# Projection Protocol

Purpose: serve controlled read models over core residue.

## Request

```text
projection_ref
case_ref
audience
projection_kind
scope
redaction_policy_refs
freshness_requirement
requested_at
```

## Result

```text
projection_ref
projection_kind
source_record_refs
source_receipt_refs
source_graph_refs
memory_refs
freshness
provenance
redaction_applied
payload
created_at
```

## Rule

Projection is not truth. Projection is a controlled read model over receipts, records, graph, memory and policy posture.
