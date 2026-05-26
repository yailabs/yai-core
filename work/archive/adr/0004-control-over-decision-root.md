# ADR 0004: Control Over Decision Root

## Status

Accepted for DOC.NEW.1.

## Context

`decision` is too narrow as a root. The machine needs to own policy source,
material, claims, rules, binding, conflicts, precedence, gates, admission,
decisions, obligations, review posture and receipt requirements in one control
plane.

## Decision

`control` is the root. `decision` is an object inside control.

## Consequences

Policy source, material, claim, rule, binding, conflict, precedence, gate,
admission, decision, obligation and review posture live in one machine control
plane. Policy is enforceable only after it becomes a machine gate, constraint,
obligation, receipt requirement, projection rule, retention rule or failure
mode.

## Non-goals

Do not treat policy text as enforcement. Do not recreate `decision` as a future
root. Do not let projections or carriers invent authority decisions.

## Supersedes

The old `src/decision` root as the future owner of control semantics.

## Related docs

```text
../architecture/06-control-policy-model.md
../protocols/control-decision.md
../protocols/policy-materialization.md
```
