# LMDB Record Plane

SPINE.29 defines the durable record lookup plane.

SPINE.30 LMDB Record Write Path adds the first LMDB write path while preserving
journal replay/audit.

SPINE.31 LMDB Record Read / Query Path adds direct CLI reads over the LMDB id,
case and kind indexes.

SPINE.32 LMDB Case / Subject / Receipt Indexes adds derived subject and receipt
indexes over structured record fields.

SPINE.33 LMDB CLI + Manual Validation freezes the operator command output and
manual validation matrix.

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
`schema_meta`. SPINE.32 implements `records_by_subject` and
`records_by_receipt`. Remaining projection/time/meta indexes stay planned.

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

## SPINE.31 Read / Query Path

SPINE.31 makes the record plane inspectable from the CLI:

```text
yai store record get <record_id>
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
```

`get` reads `records_by_id`. `list --case` scans `records_by_case` and resolves
matching record ids through `records_by_id`. `list --kind` scans
`records_by_kind` and resolves matching record ids through `records_by_id`.

The read path is intentionally small. It does not add a query planner, journal
replay, subject/receipt/projection/time indexes or graph/fact/memory joins.
Missing records return `record: not_found`. Missing, uninitialized or
unavailable LMDB environments return record-store status fields and do not
fabricate journal-backed reads.

## SPINE.32 Subject / Receipt Indexes

SPINE.32 adds:

```text
records_by_subject record:subject:<subject_ref>:<record_id> -> record_id
records_by_receipt record:receipt:<receipt_ref>:<record_id> -> record_id
```

Only structured `subject_ref` and `receipt_id` fields are indexed. The writer
does not parse free-text payloads and does not invent refs.

The CLI reads these indexes through:

```text
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
```

No-result queries return `records_total: 0` and `records: none`. Missing,
uninitialized or unavailable LMDB environments still return record-store status
fields and do not fall back to journal.

## SPINE.33 CLI Validation

SPINE.33 freezes the existing LMDB command surface:

```text
yai store status
yai store summary
yai store record get <record_id>
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
```

List commands use a consistent shape:

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

Missing records print `record: not_found`. Missing, uninitialized or
unavailable LMDB environments print record-store status fields and do not read
from journal as a fallback.

## Delivery Boundary

SPINE.30 adds the LMDB dependency, opens the environment, writes records by id
and minimal case/kind indexes, and exposes aggregate summary counts. SPINE.31
adds record get/list over those indexes. SPINE.32 adds subject/receipt
secondary indexes. SPINE.33 freezes CLI/manual validation. SPINE.34 freezes
the LMDB record plane before journal replay hardening. It does not add journal
replay, graph/fact/memory backends, projection/time indexes or projection
deltas.

## SPINE.34 LMDB Record Plane Freeze

```text
LMDB is durable indexed record lookup.
journal remains replay/audit.
no journal fallback.
```

Frozen indexes:

```text
records_by_id
records_by_case
records_by_kind
records_by_subject
records_by_receipt
schema_meta
```

The freeze verifies that the record plane can preserve carrier/control
material such as:

```text
carrier_request
effect_receipt
filesystem_receipt
divergence
reconciliation
```

without adding new carrier execution.

Next:

```text
SPINE.35 Journal Replay Doctrine + Parser Hardening done
SPINE.36 Journal Replay to LMDB done
SPINE.37 Replay Idempotency + Schema Version Handling
```

SPINE.35 adds `yai journal inspect --path <journal.jsonl> [--show-errors]`.
It is parser/readiness inspection only. Journal is replay/audit. LMDB is
durable indexed record lookup. The command reports replay diagnostics and does
not write LMDB or create a no silent journal fallback path.

SPINE.36 adds `yai journal replay --path <journal.jsonl> [--dry-run]`. Replay
uses the existing LMDB writer and frozen indexes. Existing records are counted
as `records_duplicate`, so basic idempotent replay is visible without changing
store query semantics.
