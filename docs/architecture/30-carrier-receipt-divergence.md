# Carrier Receipt Divergence

Status: SPINE.33I.

Purpose: define carrier receipt consistency as a reconciled relationship.

## Decision

YAI treats carrier receipt consistency as a relationship between decision,
dispatch, carrier outcome, receipt posture and observation result.

Inconsistency becomes divergence posture, with no silent repair and no silent
acceptance.

## Consistency Chain

```text
decision
-> dispatch
-> carrier_attempted
-> carrier outcome
-> receipt posture
-> observation result
-> divergence candidate
```

SPINE.33I does not execute carriers. It classifies whether a posture is
consistent.

## Divergence Posture

The reconciler makes these cases visible:

```text
denied_but_attempted
executed_without_receipt
blocked_but_effect_observed
receipt_claimed_executed_but_not_observed
failed_with_partial_effect
skeleton_executed_unexpectedly
```

Clean blocked, executed and observed scenarios remain consistent.

## Boundary

This is not receipt persistence, graph repair, LMDB behavior or new carrier
execution. It is the consistency layer that SPINE.34 can depend on before the
record plane freeze.

## Operator Review Link

SPINE.44A makes review/deferred/quarantine posture active. `require_review`
creates a `pending_operator` item. Approve may continue to the filesystem
carrier only when the sandbox target is safe. Deny, defer and quarantine keep
`carrier_attempted: false` and `execution_performed: false` and create explicit
receipt residue. `subject:linenoise-terminal is prompt surface`; operator
reviewer authority is separate. The active lab path is
`docs/labs/filesystem-loop`.
