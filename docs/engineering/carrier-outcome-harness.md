# Carrier Outcome Harness

Status: SPINE.33H.

Purpose: track the carrier outcome harness implementation.

## Summary

The carrier outcome harness validates outcome posture across active, skeleton
and unsupported carrier families without performing carrier effects.

This is outcome posture testing. It is not carrier execution.

## Files

```text
include/yai/effect/carrier_outcome_harness.h
system/effect/carrier_outcome_harness.c
tests/smoke/carrier-outcome-harness/test_carrier_outcome_harness.c
tools/checks/check-carrier-outcome-harness.sh
```

## Outcome Taxonomy

```text
executed
blocked
deferred
failed
mismatch
observed
quarantined
waiting_operator
waiting_agent
not_attempted
unknown
```

## CLI

```bash
yai carrier outcome-test --family database --outcome blocked
yai carrier outcome-test --family network_http --outcome failed
yai carrier outcome-test --family repository_git --mode observed --outcome mismatch
yai carrier outcome-test --family model_provider --outcome waiting_operator
yai carrier outcome-test --family review --outcome waiting_agent
```

Expected skeleton posture:

```text
execution_performed: false
carrier_attempted: false
receipt_required: yes
receipt_posture: simulated
```

`mismatch` reports:

```text
divergence_candidate: generated
```

skeleton carriers must never execute. The harness proves that outcome states
are visible and receiptable across the carrier matrix before receipt and
divergence hardening.

## Validation

```bash
make check-carrier-outcome-harness
make smoke-spine33h
```
