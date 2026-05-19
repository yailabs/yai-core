# ADR 0002: Case-Bound Subjects

## Status

Accepted for DOC.NEW.1.

## Context

The earlier core model could describe material, cases and effects, but it did
not make the operational entity being observed or controlled explicit enough.
Without first-class subjects, a case becomes a stream rather than an operational
world.

## Decision

Subjects are first-class primitives. A subject is any observable, controllable,
invokable, queryable or targetable entity bound to a case.

## Consequences

Cases govern bound subjects. Operations target subjects. Carriers act on
subjects. Receipts update subject state. Memory can remember subject behavior
over time. Identity, locator, binding mode, control mode and state posture must
be modeled separately.

## Non-goals

Do not bury subjects inside case attachments or runtime observations. Do not
treat locator as identity. Do not claim complete external system truth from a
subject binding alone.

## Supersedes

Subject-as-attachment and subject-as-runtime-observation interpretations.

## Related docs

```text
../architecture/04-subject-model.md
../protocols/subject-binding.md
../architecture/00-spine.md
```
