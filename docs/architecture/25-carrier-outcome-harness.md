# Carrier Outcome Harness

Status: SPINE.33H.

Purpose: define carrier outcome posture testing across the carrier matrix.

## Decision

YAI tests carrier outcome posture independently of real carrier execution.
Skeleton carriers may simulate outcome posture but must never execute effects.

## Outcome Scope

The harness represents:

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

`unknown` is internal/error posture. Manual CLI validation uses unsupported
input and unsupported families instead of treating `unknown` as a normal
operator request.

## Execution Boundary

Outcome harness is not carrier execution.

Active carriers may expose existing posture, but the CLI harness remains
dry-run. Skeleton carriers always report:

```text
execution_available: false
execution_performed: false
carrier_attempted: false
receipt_posture: simulated
```

`mismatch` generates a divergence candidate marker. SPINE.33I hardens receipt
and divergence consistency after this posture surface exists.
