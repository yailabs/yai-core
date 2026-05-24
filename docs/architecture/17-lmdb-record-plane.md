# LMDB Record Plane

SPINE.29 defines the durable record lookup plane.

SPINE.30 LMDB Record Write Path adds the first LMDB write path while preserving
journal replay/audit.

Doctrine:

```text
hot state = live cache / freshness / current view
journal   = replay / audit
LMDB      = durable indexed record lookup
graph     = causal relationship plane
DuckDB    = analytical fact plane
memory    = operational consolidation
```

LMDB is durable indexed record lookup. It is not hot state, graph, analytics or
memory. hot state is not LMDB. The journal remains replay/audit and remains the
source used to rebuild record lookup while append-only residue is active.

LMDB must never become the only copy of replay or audit truth. A missing LMDB
environment can be rebuilt from journal. A corrupt LMDB environment must not
destroy audit truth.

## Runtime Layout

Target runtime layout:

```text
YAI_HOME/
└── store/
    ├── journal/
    ├── lmdb/
    │   ├── records.mdb
    │   ├── lock.mdb
    │   └── README
    ├── graph/
    ├── facts/
    └── memory/
```

SPINE.29 defines and may create only:

```text
YAI_HOME/store/lmdb/
```

SPINE.30 opens that directory as an LMDB environment, writes schema metadata
and stores indexed record copies.

## Environment And Namespaces

The logical environment name is:

```text
record_env
```

Named DBs or logical namespaces:

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
records_by_projection
records_by_time
record_meta
schema_meta
```

SPINE.30 implements `records_by_id`, `records_by_case`, `records_by_kind` and
`schema_meta`. Remaining indexes stay planned for SPINE.32+.

## Key Grammar

Logical key grammar:

```text
record:id:<record_id> -> record envelope
record:case:<case_ref>:<record_id> -> record_id
record:kind:<kind>:<record_id> -> record_id
record:subject:<subject_ref>:<record_id> -> record_id
record:receipt:<receipt_ref>:<record_id> -> record_id
record:projection:<projection_ref>:<record_id> -> record_id
record:time:<unix_ms>:<record_id> -> record_id
meta:schema -> schema version
meta:rebuild -> rebuild status
```

Future binary keys may encode this grammar compactly, but they must remain
deterministic, inspectable and documented.

## Value Envelope

Record values are versioned. The minimum envelope is:

```json
{
  "schema": "yai.record.v1",
  "record_id": "rec:...",
  "record_kind": "...",
  "case_ref": "case:...",
  "created_at_unix_ms": 0,
  "source": {
    "plane": "journal",
    "ref": "..."
  },
  "payload": {}
}
```

Full value families may add:

```text
subject_refs
provenance_refs
receipt_refs
projection_refs
payload
```

JSON describes the v1 envelope. It does not require JSON as the permanent LMDB
storage encoding.

## Record Families

SPINE.29 names the initial record families so the write path does not invent a
taxonomy ad hoc:

```text
case_domain
case_attachment
case_binding
subject_binding
subject_state
policy_rule
projection_rule
authority_scope
interaction_thread
interaction_turn
participant_view_frame
attempt
decision
decision_basis
obligation
receipt_requirement
receipt
filesystem_receipt
graph_edge
memory_candidate
divergence
reconciliation
projection
projection_request
projection_result
query_result
model_interpretation
hot_state_snapshot
pack_manifest
pack_material
```

`hot_state_snapshot` is diagnostic only; it must not make LMDB authoritative
for liveness or freshness.

## Plane Relationships

Hot state stores current liveness/freshness. LMDB stores durable record lookup.
Hot state may later point to `last_record_id` or `last_receipt_id` stored in
LMDB, but LMDB does not decide projection freshness.

Ladybug stores causal graph. DuckDB stores derived facts. Memory stores
consolidated operational experience. Projection reads from records, graph,
memory and hot state as needed, but does not move their ownership into LMDB.

LMDB may contain refs used by graph, facts and memory. It must not own their
derived semantics.

## SPINE.30 Write Path

SPINE.30 writes normalized record envelopes into LMDB after daemon loop journal
records have been written. The journal remains append-only chronology and
replay/audit source. LMDB receives durable indexed record copies.

Minimum indexes:

```text
records_by_id      record:id:<record_id> -> yai.record.v1 envelope
records_by_case    record:case:<case_ref>:<record_id> -> record_id
records_by_kind    record:kind:<record_kind>:<record_id> -> record_id
schema_meta        meta:schema -> yai.record.v1
schema_meta        meta:rebuild -> not_started
```

`ready` means the LMDB environment opens and `meta:schema` is present with
`yai.record.v1`. It does not mean replay, read/query, graph, facts or memory
are implemented.

LMDB write failure is explicit. A daemon loop response still names the journal
path; the CLI then reports record-store import failure instead of silently
pretending the record plane is current.

## Delivery Boundary

SPINE.30 adds the LMDB dependency, opens the environment, writes records by id
and minimal case/kind indexes, and exposes aggregate summary counts. It does
not add record get/list query commands, journal replay, graph/fact/memory
backends or projection deltas.

Next:

```text
SPINE.31 LMDB Record Read / Query Path
SPINE.32 LMDB Case / Subject / Receipt Indexes
SPINE.33 LMDB CLI + Manual Validation
SPINE.34 LMDB Record Plane Freeze
```
