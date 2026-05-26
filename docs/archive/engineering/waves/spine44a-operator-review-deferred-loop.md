Historical/superseded engineering record. Not an active source of truth.

# SPINE.44A Operator Review / Deferred Action Loop v0

Status: delivered.

SPINE.44A adds a real pending operator loop for filesystem sandbox writes. The
loop creates a denied outside-sandbox write and a pending inside-sandbox
`require_review` item. `yai control pending` lists the `pending_operator`
request, `yai control show` exposes the authority boundary, and approve, deny,
defer and quarantine resolve the request with explicit receipts.

The automation/test contract is non-interactive: the loop emits
`review_required: yes`, `status: pending_operator` and `next_commands`, then
exits.

`subject:linenoise-terminal is prompt surface`; operator reviewer authority is
separate and local-dev scoped.

Validation:

```bash
make check-operator-review-loop
make smoke-spine44a
```

Guard vocabulary: require_review, pending_operator, approve, deny, defer,
quarantine, carrier_attempted: false, execution_performed: false,
subject:linenoise-terminal is prompt surface, operator reviewer authority,
docs/labs/filesystem-loop.
