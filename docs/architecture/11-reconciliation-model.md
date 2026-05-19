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
