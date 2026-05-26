# ADR 0027: Carrier Outcome Harness

Status: accepted

## Context

SPINE.33F made carrier coverage visible. SPINE.33G made non-process carrier
families inspectable as skeletons. The next gap is outcome posture: every
carrier family must expose how outcomes are represented without implying that
unsupported carriers can execute effects.

## Decision

YAI tests carrier outcome posture independently of real carrier execution.
Skeleton carriers may simulate outcome posture but must never execute effects.

## Consequences

- `yai carrier outcome-test` exposes dry-run outcome posture.
- Skeleton carrier tests report `execution_performed: false`.
- `mismatch` may create a divergence-candidate marker.
- SPINE.33I can harden receipt and divergence consistency against a complete
  outcome matrix.
