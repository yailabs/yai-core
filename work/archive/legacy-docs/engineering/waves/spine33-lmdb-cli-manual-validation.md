Historical/superseded engineering record. Not an active source of truth.

# SPINE.33 LMDB CLI + Manual Validation

Status: completed
Owner: engineering/data-plane
Purpose: Archive the record-plane CLI validation wave.
Not source of truth for: current command output; see active command-surface and testing docs.

## Summary

SPINE.33 freezes the existing LMDB record-plane CLI before the record-plane
freeze. It adds no new record semantics and no journal replay path.

Validated commands:

```text
yai store status
yai store summary
yai store record get <record_id>
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
```

The frozen list shape is:

```text
filter: case|kind|subject|receipt
filter_value: ...
records_total: N
limit: N
records:
- record_id: ...
  record_kind: ...
  case_ref: ...
```

Zero-result lists print `records: none`. Missing records print
`record: not_found`. Missing, uninitialized or unavailable LMDB stores print
record-store status fields and do not fall back to journal.

## Validation

The wave adds:

```text
make smoke-spine33
```

The smoke exercises the installed-style manual workflow over daemon loop
imports, including status before write, ready status after write, summary
counts, list by case/kind/subject/receipt, get by id, missing record and
zero-result query.

## Boundary

No new indexes, read planner, graph backend, DuckDB backend, memory
consolidation, projection delta, model/provider work or legal cleanup was
introduced.
