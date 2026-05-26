# CLI Review Interaction Surface

Status: SPINE.44B CLI Review Interaction Surface.

SPINE.44B makes the SPINE.44A review loop usable from terminal automation.
The backend creates durable `review_request`, `review_decision` and
`control_pending` state. The CLI renders and resolves that state. Backend
operations must not block on random terminal input.

The CLI modes are:

```text
automation = control pending/show print review_id, next_commands and exit
interactive = yai control review --interactive prompts only when TTY is present
watch = yai control watch polls review/control state changes
wait = yai control wait waits for a review resolution or timeout
```

Plain output remains stable and parseable. Pretty and JSON formats are reserved
for later, but the v0 contract stays line-oriented.

Authority boundary:

```text
subject:linenoise-terminal is prompt surface
subject:operator-reviewer is review authority
```

`approve` may execute the reviewed filesystem carrier. `deny`, `defer` and
`quarantine` keep `carrier_attempted: false` and `execution_performed: false`.

The active lab path is `docs/labs/filesystem-loop`.

Guard vocabulary: control pending, control show, control review --interactive,
control watch, control wait, next_commands, not_a_tty, pending_operator,
approve, deny, defer, quarantine, carrier_attempted: false,
execution_performed: false, subject:linenoise-terminal is prompt surface,
subject:operator-reviewer is review authority, docs/labs/filesystem-loop.

SPINE.44C adds a test/lab alignment matrix over this CLI surface. The matrix
keeps `next_commands`, `wait timeout`, bounded `watch`, approve, deny, defer,
quarantine and `pending_operator` visible. Deny, defer and quarantine keep
`carrier_attempted: false` and `execution_performed: false`.

Model-facing posture is documented in `docs/labs/filesystem-loop`: model proposal
observed, model cannot approve, and automatic proposed-op gate import is future
work. The executable CLI/control matrix remains in the same lab.

Review matrix guard: model proposal observed; model cannot approve; automatic
proposed-op gate import is future work.

SPINE.45 freezes the graph-facing visibility of CLI review records:
`review_request`, `review_decision` and `control_pending`. The CLI remains a
rendering/resolution surface over durable control state, while RuntimeGraph is
not durable truth.
