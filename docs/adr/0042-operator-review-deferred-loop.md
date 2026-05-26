# ADR 0042: Operator Review / Deferred Action Loop

Status: accepted.

## Decision

YAI represents require_review, deferred and quarantine as active control
states, not passive labels. Operations in these states must not execute until
an authorized review resolution allows them.

The v0 loop is filesystem-sandbox only. `approve` may continue to the
filesystem carrier when the reviewed target remains inside sandbox. `deny`,
`defer` and `quarantine` keep `carrier_attempted: false` and
`execution_performed: false`.

Automation/test mode is non-interactive: the review loop emits
`review_required: yes`, `status: pending_operator` and `next_commands`, then
exits.

`subject:linenoise-terminal is prompt surface`; operator reviewer authority is
held by a separate local-dev reviewer subject.

The active lab path is `docs/labs/filesystem-loop`.

Guard vocabulary: require_review, pending_operator, approve, deny, defer,
quarantine, carrier_attempted: false, execution_performed: false,
subject:linenoise-terminal is prompt surface, operator reviewer authority,
docs/labs/filesystem-loop.
