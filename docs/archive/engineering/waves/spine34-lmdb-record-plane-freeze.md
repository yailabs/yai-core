Historical/superseded engineering record. Not an active source of truth.

# SPINE.34 LMDB Record Plane Freeze

Status: completed

SPINE.34 froze the LMDB record plane after SPINE.29-SPINE.33 and before journal
replay hardening.

Delivered:

```text
yai.record.v1 freeze statement
records_by_id/case/kind/subject/receipt freeze
no journal fallback doctrine
carrier/control/divergence record-store validation
make check-lmdb-record-plane-freeze
make smoke-spine34
```

SPINE.34 did not add journal replay, graph persistence, DuckDB facts, memory,
RuntimeGraph, HNSW, projection deltas, provider runtime behavior or new carrier
execution.
