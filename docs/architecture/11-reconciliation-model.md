# Reconciliation Model

Reconciliation handles mismatch between expected, decided, executed, observed and remembered reality.

## Divergence Classes

```text
expected_vs_observed
claim_without_receipt
receipt_without_decision
external_bypass
policy_conflict
stale_memory
subject_state_mismatch
carrier_partial_failure
projection_stale
```

## Reconciliation Flow

```text
detect divergence
classify risk
link records / receipts / graph edges
produce recovery or compensation posture
project finding
```

## Rule

Reconciliation must not hide gaps. A missing receipt, bypass or conflicting policy is an explicit artifact, not an exception swallowed by projection.

## NEW.5 Reconstruction Input

NEW.5 does not implement reconciliation yet, but it creates the first input
needed by reconciliation: graph edges that connect case, subject, operation,
decision and receipt.

Receipt chain reconstruction can expose whether a receipt lacks:

```text
case edge
operation edge
decision edge
subject update edge
```

Later reconciliation can turn those gaps into explicit divergence artifacts.

## NEW.7 Divergence V0

NEW.7 adds two store record kinds:

```text
divergence
reconciliation
```

Initial divergence kinds:

```text
denied_but_executed
receipt_without_decision
claim_without_receipt
missing_target_subject
stale_subject_state
memory_without_basis
graph_chain_missing
```

Initial severity:

```text
info
warning
error
critical
```

Initial reconcile posture:

```text
observed
requires_review
recoverable
unrecoverable
compensated
```

The first detector is residue-derived. It does not execute recovery or
compensation. It only records mismatch for projection and operator review.

## Projection Interaction

NEW.8 makes divergence visibility explicit in projection results. A projection
result carries `source_divergence_count` and a redaction posture, so audit and
operator views can expose mismatch without allowing projection to rewrite or
hide reconciliation truth.
