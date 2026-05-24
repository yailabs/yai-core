# Hot State Plane

SPINE.23 implements hot state v0. SPINE.24 hardens the runtime snapshot.
SPINE.25 integrates the snapshot with case session and case context lifecycle.
SPINE.26 adds consumer-aware projection freshness policy.

Hot state is the live operational cache for a case session. It is not durable
truth and it does not replace journal, records, graph, facts, memory or
reconcile.

## Runtime Shape

```text
in-process hot state
YAI_HOME/run/hot-state.json
schema: yai.hot_state.v1
write: hot-state.json.tmp then rename
future mmap/shared-memory compatible layout
```

The snapshot path is local runtime state. It can be rebuilt from durable
residue and is safe to treat as diagnostic/cache material.

Snapshot lifecycle:

```text
missing -> hot_state: unavailable / reason: missing_snapshot
corrupt -> hot_state: unavailable / reason: invalid_snapshot
valid   -> hot_state: active
```

## Required Fields

```text
hot_state_id
case_ref
case_session_id
case_context_id
case_session_status
case_world_status
case_context_status
case_version
active_thread_id
participant_view_frame_id
current_projection_id
previous_projection_id
last_record_id
last_decision_id
last_receipt_id
last_model_interpretation_id
last_divergence_id
pending_op_count
pending_obligation_count
carrier_lock_count
projection_freshness
projection_stale_reason
dirty_flags
updated_at_unix_ms
```

## Projection Freshness

Projection freshness is the first concrete hot-state diagnostic.

Rules:

```text
new receipt -> projection stale
new decision -> projection stale
new authority scope -> projection stale
new memory -> projection stale
new divergence -> projection stale
thread switch -> projection stale
projection rebuild/refresh -> projection fresh
```

Every stale projection must carry a stale reason.

## Dirty Flags

```text
record_dirty
receipt_dirty
graph_dirty
memory_dirty
projection_dirty
authority_dirty
thread_dirty
obligation_dirty
carrier_dirty
```

Dirty flags are invalidation hints. They are not durable facts.

## Commands

```text
yai doctor
yai hot status
```

`yai doctor` reports hot-state path/readiness. `yai hot status` reads the local
snapshot and reports active case, session, context, freshness, stale reason and
latest residue refs.

SPINE.25 extends `yai hot status` with case-world, active-thread and
participant-view status:

```text
case_session: active|inactive|unknown
case_world: loaded|not_loaded|unknown
case_context: active|inactive|unknown
active_thread: <id>|none|unknown
participant_view: <frame_id>|none|unknown
```

SPINE.26 adds policy output:

```text
projection_policy: usable|refresh_recommended|refresh_required|blocked_for_model|unknown
```

Command surface:

```text
Changed commands:
  yai hot status
  yai doctor

Manual tests:
  yai doctor
  yai hot status
```

## Future Timing

SPINE.23 reserves this observability direction but does not implement timing
facts:

```text
receipt_to_hot_state_ms
hot_state_to_projection_ms
receipt_to_model_visible_ms
```

## Old-YAI Extraction

SPINE.23 mines old runtime/session/working/view concepts from old `yai`
read-only. The extracted semantics are active runtime state, session state,
working context, pending operation state, view freshness and continuity.
