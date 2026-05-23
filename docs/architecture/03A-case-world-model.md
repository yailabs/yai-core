# Case World Model

Status: SPINE.3R case-world doctrine.

Case world is the first operational layer inside a case. A case does not begin
by treating every participant as a subject. It first constructs the bounded
world in which participants, material, authority and projection posture can be
understood.

Canonical rule:

```text
Everything that participates in a case enters first as case-world material:
a domain, attachment or binding.
```

Only after attachment and binding can material act as a subject, source, actor,
target, projection consumer or carrier participant.

## Spine Position

```text
ingest
-> case_world
-> case_domain
-> case_attachment
-> case_binding
-> case_session
-> case_context
-> subject
-> policy_rule
-> projection_rule
-> authority_scope
-> live_projection
-> model_interpretation / claim / attempt
-> control
-> effect / carrier
-> receipt
-> journal
-> record store
-> graph
-> index/query
-> memory
-> reconcile
-> next projection delta
```

## Record Families

`case_domain`
: Bounded operational domain inside the case.

`case_attachment`
: Material, resource, process, provider, model, terminal, policy pack,
operator or environment process attached to the case world.

`case_binding`
: Relationship between the case and attached entity, including binding mode,
control mode, authority scope and projection posture.

`projection_rule`
: What a subject, model, operator or interface may receive or produce
cognitively.

`authority_scope`
: What authority a subject is granted inside the case.

`model_interpretation`
: A model interpretation of projected case residue. It is not authoritative
truth.

## Authority Boundaries

Not everything attached has authority.

```text
model interpretation is not case truth
terminal interface is not execution authority
provider is not policy truth
ai-environment process is not core owner
```

A provider, model, terminal, operator, filesystem sandbox and policy pack can
all be attached to the case world. Their authority is still determined by
binding posture, policy, projection rules, authority scope, control decisions
and receipts.

## Current Implementation Posture

NEW.18A case-world binding records already exist in the current repository
history as the `case_world` vertical slice:

```text
include/yai/case/case_world.h
system/case/case_world.c
record_kind: case_domain
record_kind: case_attachment
record_kind: case_binding
```

SPINE.3R does not add source movement or new backend implementation. It makes
the case-world layer canonical before NEW.19 guard realignment.

NEW.18B adds the live case context boundary after case-world materialization.
The case world is materialized from records; the active runtime operates on
`case_context` inside a `case_session`.

SPINE.4 adds an observability obligation over case-world material. Attachments,
bindings, projection posture and authority scope must be measurable for
freshness, provenance sufficiency, authority alignment and divergence exposure.
