# Operator Review / Deferred Action Loop

Status: SPINE.44A Operator Review / Deferred Action Loop v0.

SPINE.44A makes `require_review`, `defer` and `quarantine` active control
states. They are not passive labels. A controlled operation that enters one of
these states must not execute until an authorized review resolution allows it.

Core rule:

```text
blocked = final denial, no execution
require_review = pending_operator authority, no execution
defer = pending condition, no execution
quarantine = isolated, no execution
approve = may continue to carrier
deny = final blocked receipt
```

The v0 scope is filesystem sandbox only. The loop creates an outside-sandbox
write attempt that is denied immediately and an inside-sandbox write attempt
that becomes a pending review request.

Automation/test mode is non-interactive. A pending review emits
`review_required: yes`, `status: pending_operator`, `carrier_attempted: false`,
`execution_performed: false` and `next_commands`, then the command exits.

`subject:linenoise-terminal is prompt surface`. It does not own approval
authority. `subject:operator-reviewer` owns operator reviewer authority only in
the local-dev authority scope.

The active lab path is `labs/filesystem-loop`.

SPINE.44B layers the CLI review interaction surface over this durable state:
`control pending`, `control show`, `control review --interactive`,
`control watch`, `control wait`, `next_commands` and `not_a_tty`. The backend
still does not block on terminal input. `subject:operator-reviewer is review
authority`.

Guard vocabulary: require_review, pending_operator, approve, deny, defer,
quarantine, carrier_attempted: false, execution_performed: false,
subject:linenoise-terminal is prompt surface, operator reviewer authority,
labs/filesystem-loop.

SPINE.44C adds the review loop test matrix without changing this architecture.
The matrix proves blocked outside-sandbox writes, `pending_operator`,
`next_commands`, approve, deny, defer, quarantine, `wait timeout`, bounded
`watch` and non-TTY interactive handling. Non-execution states keep
`carrier_attempted: false` and `execution_performed: false`.

Model-facing posture is documented in `labs/filesystem-loop`: model
proposal observed, model cannot approve, and automatic proposed-op gate import
is future work. The executable control matrix remains in the same lab.

Review matrix guard: model proposal observed; model cannot approve; automatic
proposed-op gate import is future work.

SPINE.45 freezes graph visibility for the operator review loop. Approve is
graph/query visible through review/control relations. Deny, defer and
quarantine continue to preserve `carrier_attempted: false` and
`execution_performed: false`; RuntimeGraph is not durable truth.
