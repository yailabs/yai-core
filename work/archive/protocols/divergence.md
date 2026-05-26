# Divergence Protocol

Purpose: record mismatch between expected, decided, executed, observed and remembered state.

## Fields

```text
divergence_ref
case_ref
divergence_kind
expected_ref
observed_ref
subject_refs
op_ref optional
decision_ref optional
receipt_ref optional
severity
recovery_hint
created_at
```

## Initial Kinds

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

## Rule

Divergence is an artifact, not a logging side effect.
