# ADR 0013: Case World Before Subject Binding

## Status

Accepted for SPINE.3R.

## Context

The early core spine treated subject binding as the first case-local layer.
The current implementation has introduced `case_world` records for domain,
attachment and binding. That makes case-world construction foundational rather
than accessory.

## Decision

YAI Core constructs case-world material before subject behavior.

```text
ingest -> case_world -> case_domain -> case_attachment -> case_binding -> subject
```

Everything that participates in a case enters first as case-world material: a
domain, attachment or binding. Only after attachment and binding can it act as
subject, source, actor, target, projection consumer or carrier participant.

## Consequences

Subject binding remains a core primitive, but it is not the first case layer.
Provider routes, models, terminals, policy packs, operators, filesystem
sandboxes and environment processes are attached and bound before authority is
claimed.

Attachment alone does not grant authority.
