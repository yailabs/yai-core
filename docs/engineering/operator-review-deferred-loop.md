# Operator Review / Deferred Action Loop

Status: SPINE.44A Operator Review / Deferred Action Loop v0.

This wave adds the first low-level pending-control loop. A filesystem write
inside the sandbox can pause as `require_review`, expose a `pending_operator`
review item, and later resolve through `approve`, `deny`, `defer` or
`quarantine`.

Command surface:

```bash
yai daemon run-filesystem-review-loop --socket <socket>
yai control pending --case <case_ref>
yai control show <review_id>
yai control approve <review_id> --reason "<reason>"
yai control deny <review_id> --reason "<reason>"
yai control defer <review_id> --reason "<reason>"
yai control quarantine <review_id> --reason "<reason>"
```

Automation/test mode is non-interactive: `run-filesystem-review-loop` prints
the pending review contract and exits.

```text
review_required: yes
review_id: review:new12-fs-write-review
status: pending_operator
carrier_attempted: false
execution_performed: false
next_commands:
approve: yai control approve review:new12-fs-write-review --reason "..."
deny: yai control deny review:new12-fs-write-review --reason "..."
defer: yai control defer review:new12-fs-write-review --reason "..."
quarantine: yai control quarantine review:new12-fs-write-review --reason "..."
```

Approve writes the reviewed sandbox file and creates an executed filesystem
receipt. Deny creates a blocked receipt. Defer and quarantine keep
`carrier_attempted: false` and `execution_performed: false`; quarantine also
marks case quarantine scope.

`subject:linenoise-terminal is prompt surface`. Operator reviewer authority is
represented by `subject:operator-reviewer` in the local-dev authority scope.

Validation:

```bash
make check-operator-review-loop
make smoke-spine44a
```

The active lab path is `docs/labs/filesystem-loop`.

SPINE.44B adds the CLI review interaction surface over these records:
`control pending`, `control show`, `control review --interactive`,
`control watch`, `control wait`, `next_commands` and `not_a_tty`. The backend
creates durable state and exits; the CLI renders and resolves it.
`subject:operator-reviewer is review authority`.

Guard vocabulary: require_review, pending_operator, approve, deny, defer,
quarantine, carrier_attempted: false, execution_performed: false,
subject:linenoise-terminal is prompt surface, operator reviewer authority,
docs/labs/filesystem-loop.

SPINE.45 freezes graph visibility for this control loop. Approve is visible in
graph/query output. Deny, defer and quarantine remain no-execution states and
preserve `carrier_attempted: false` and `execution_performed: false`.
