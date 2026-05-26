Historical/superseded engineering record. Not an active source of truth.

# Case World Verticalization

Status: SPINE.5 WORLD spine engineering doctrine, based on SPINE.3R.

Case-world verticalization makes `case_world` the first case-local structure
that receives operational material. Subjects remain first-class, but subject
binding is no longer the first case layer.

## Vertical Slice

The current case-world slice contains:

```text
case_domain
case_attachment
case_binding
projection_rule
authority_scope
model_interpretation
```

The intended flow is:

```text
ingest material
classify case domain
attach participant or resource
bind attached entity to the case
declare projection and authority posture
then expose subject/source/actor/target behavior
```

NEW.18B inserts live runtime materialization before subject behavior:

```text
case_world
-> case_session
-> case_context
-> subject/source/actor/target behavior
```

## Participant Handling

Case-world material can represent:

```text
provider
model
terminal
policy pack
filesystem sandbox
operator
local daemon
ai-environment process
external resource
```

Attachment alone does not grant authority. A model can be attached and still
only produce claims or proposals. A terminal can be attached and still remain a
display/input surface. A provider route can be attached and still provide no
policy truth.

## Guard Implication

NEW.19 aligned guards with this order:

```text
case_world before subject binding
projection and authority posture before model interpretation
receipts and records before memory or reconcile claims
```

No guard should assume that the first case layer is always subject binding.
Subject binding is valid only after the participant has entered the case world
as a domain, attachment or binding.

`case_ref` and `subject_ref` remain required persistence and boundary refs.
They are not the active runtime case or subject by themselves.

SPINE.5 tracks this work under `WORLD` and requires future case-world changes
to declare related `VIEW`, `RESIDUE`, `CONTROL` and `OBSERVABILITY` coverage.
