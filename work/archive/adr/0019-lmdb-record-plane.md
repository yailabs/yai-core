# ADR 0019: LMDB Record Plane

Status: Accepted.

## Context

SPINE.28 froze hot state as a live, non-authoritative cache. The next data
plane needs durable indexed record lookup without confusing that lookup with
hot state, journal replay, graph truth, analytical facts or memory.

## Decision

LMDB is the durable indexed record lookup plane. LMDB is durable indexed record lookup.

It is not hot state, graph, analytics or memory. It is rebuildable from journal
and provides stable lookup for case records.

The logical environment is `record_env` under `YAI_HOME/store/lmdb`. Values are
versioned with schema `yai.record.v1`. Logical key prefixes include `record:id`,
`record:case`, `record:kind`, `record:subject`, `record:receipt`,
`record:projection`, `record:time`, `meta:schema` and `meta:rebuild`.

## Consequences

The journal remains replay/audit. LMDB may be missing or corrupt without
destroying audit truth.

hot state is not LMDB. Hot state may later hold last-record pointers, but LMDB
does not decide liveness, current view or projection freshness.

Graph, facts and memory may reference LMDB records, but LMDB does not own causal
semantics, analytical facts or memory consolidation.

SPINE.30 implements the first write path against this doctrine without adding
record taxonomy or keyspace decisions ad hoc. Read/query APIs remain a later
decision surface.
