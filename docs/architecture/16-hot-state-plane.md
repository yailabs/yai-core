# Hot State Plane

SPINE.23 introduces the first live data plane. SPINE.24 hardens the runtime
snapshot lifecycle.

Hot state is not truth. It is the live operational cache of an active case
session: what case is current, which context and thread are active, which
projection frame is current, which residue changed after that projection and
whether the next model/operator view is fresh or stale.

The journal remains replay/audit. Durable truth remains in residue: journal and
records today, later LMDB, graph, facts, receipts, memory and reconcile.

## Boundary

Hot state may answer:

```text
active case_ref
active case_session_id
active case_context_id
case_version
active_thread_id
current_projection_id
previous_projection_id
last record/decision/receipt/model_interpretation/divergence
pending operations
pending obligations
carrier locks
projection freshness
projection stale reason
dirty flags
```

Hot state must not answer:

```text
complete history
durable record truth
full graph
full memory
audit packet
policy truth
```

## Freshness

Projection freshness is explicit:

```text
fresh
stale
unknown
rebuilding
```

Stale reason is also explicit:

```text
new_receipt_after_projection
new_decision_after_projection
new_authority_scope_after_projection
new_memory_after_projection
new_divergence_after_projection
thread_changed
manual_refresh_required
unknown
```

## V0 Backend

SPINE.23 uses in-process hot state plus a daemon-owned local snapshot:

```text
YAI_HOME/run/hot-state.json
```

This creates the semantic boundary and a file-backed, mmap-ready layout
contract. True OS shared memory is a later optimization, not a requirement for
the v0 doctrine.

The snapshot schema is:

```text
yai.hot_state.v1
```

The daemon writes through a temporary file and then renames it into place.
Readers must handle missing and corrupt snapshots without crashing.

## Lifecycle

When a case session becomes active, hot state is initialized from the loaded
case session and context. New decisions and receipts mark projection stale.
Projection rebuild marks it fresh. Thread switch marks it stale with
`thread_changed`.

The daemon owns hot-state lifecycle for local runtime execution. The CLI may
read the snapshot for diagnostics.

Snapshot status:

```text
missing_snapshot -> hot_state unavailable
invalid_snapshot -> hot_state unavailable
valid snapshot -> hot_state active
```
