# Carrier Receipt Divergence

Status: SPINE.33I.

Purpose: track carrier receipt / divergence hardening.

## Summary

SPINE.33I adds a carrier consistency primitive and CLI harness. It checks
whether decision, dispatch, carrier outcome, receipt posture and observation
result agree.

If they do not agree, YAI reports a divergence candidate. There is no silent
repair. The active rule is no silent repair and no silent acceptance.

## Files

```text
include/yai/reconcile/carrier_consistency.h
system/reconcile/carrier_consistency.c
tests/smoke/carrier-receipt-divergence/test_carrier_receipt_divergence.c
tools/checks/check-carrier-receipt-divergence.sh
```

## Consistency Model

```text
decision
dispatch
carrier outcome
receipt posture
observation result
divergence candidate
```

The command surface is diagnostic:

```bash
yai carrier reconcile-outcome --scenario clean_blocked
yai carrier reconcile-outcome --scenario denied_but_attempted
yai carrier reconcile-outcome --scenario executed_without_receipt
yai carrier reconcile-outcome --scenario blocked_but_effect_observed
yai carrier reconcile-outcome --scenario skeleton_executed_unexpectedly
```

The harness does not execute carriers.

## Divergence Kinds

```text
denied_but_attempted
blocked_but_effect_observed
executed_without_receipt
receipt_claimed_executed_but_not_observed
failed_with_partial_effect
missing_receipt
receipt_without_decision
decision_without_receipt
carrier_timeout
unsafe_target_attempted
skeleton_executed_unexpectedly
none
unknown
```

## Expected Output

Denied but attempted:

```text
scenario: denied_but_attempted
decision: deny
carrier_attempted: true
execution_performed: true
divergence_candidate: denied_but_attempted
severity: critical
result: inconsistent
```

Clean blocked:

```text
scenario: clean_blocked
decision: deny
carrier_attempted: false
execution_performed: false
receipt_present: yes
divergence_candidate: none
severity: info
result: consistent
```

## Relationship To Outcome Test

`yai carrier outcome-test` represents posture.

`yai carrier reconcile-outcome` checks whether posture is coherent against
decision, dispatch, receipt posture and observation result.

## Validation

```bash
make check-carrier-receipt-divergence
make smoke-spine33i
```
