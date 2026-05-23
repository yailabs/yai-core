# Live Case Context

Status: NEW.18B live case context boundary.

NEW.18B closes the case layer before NEW.19. It preserves durable refs while
making the active runtime case a loaded context, not only a set of CLI
arguments.

Core rule:

```text
refs identify durable material
handles, sessions and contexts operate on live loaded case state
```

## Terms

`case_id`
: Durable identity.

`case_ref`
: Serializable external and persistent reference. It belongs in records,
journals, graph edges, receipts, IPC and audit output.

`case_handle`
: In-process handle to a loaded case object. This is not serialized as durable
truth.

`case_world`
: Materialized operational world of a case: domains, attachments, bindings,
subjects, authority scopes, projection rules and policy material refs.

`case_context`
: Active runtime view used by control, projection, carrier and model/provider
interaction.

`case_session`
: Daemon or CLI active case instance with journal backing path, case world,
context and lifecycle state.

## Boundary

```text
journal stores refs
runtime operates on case_context
daemon manages case_session
projection reads from case_world/context
participant view reads from active thread + projection frame
```

`case_context` is not truth. Durable truth still comes from residue:

```text
journal
record store
receipts
graph
memory
divergence
```

## Entering A Case

`yai case enter` means:

```text
load or create active case_session
materialize case_world from journal/current records
derive case_context
expose participant/model/operator view
```

The current implementation remains journal-file backed. The journal path is the
case session backing store. `case_ref` is persistent identity. `case_context` is
the active operating surface.

There is no global singleton case. Multiple sessions must remain possible.

NEW.18C adds an active interaction thread below the case session. A case session
can switch threads without deleting journal or case history. The active
participant view frame is derived from the selected thread plus the current
case projection.

## Observability

SPINE.4 treats a live case context as measurable, not inherently trustworthy.
The context must expose enough posture for Case View Quality checks:

```text
active case version
latest delta
projection freshness
authority alignment
case_context_consistency
pending obligations
critical divergence flags
```

`case_context` remains runtime materialization. It is evaluated by
observability; it does not become truth.
