# LMDB Record Plane

SPINE.29 defines the durable record lookup plane before any LMDB write path is
implemented.

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

The command and install path do not open LMDB, create records or claim
readiness.

## Environment And Namespaces

The logical environment name is:

```text
record_env
```

Future named DBs or logical namespaces:

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

If SPINE.30 begins with a single physical DB, these names still define logical
prefix ownership.

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

## Delivery Boundary

SPINE.29 adds doctrine, schema, keyspace and status visibility. It does not add
an LMDB dependency, open an environment, write records, read records, replay the
journal or build graph/fact/memory backends.

Next:

```text
SPINE.30 LMDB Record Write Path
SPINE.31 LMDB Record Read / Query Path
SPINE.32 LMDB Case / Subject / Receipt Indexes
SPINE.33 LMDB CLI + Manual Validation
SPINE.34 LMDB Record Plane Freeze
```
