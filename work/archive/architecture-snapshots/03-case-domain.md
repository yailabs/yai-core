# Case Domain

The case is the operational control domain.

A case first builds a case world, then opens a live case session/context, then
binds subjects, policy, evidence, procedure posture and closure. It gives
operations a bounded world in which control can be evaluated and effects can be
reconstructed.

## Owns

```text
case identity
case boundary
case world
case domain
case attachment
case binding
case session
case context
case subject binding
case posture
case materialization
case evidence refs
case policy binding
case procedure record
case closure
```

## Does Not Own

```text
subject semantics
carrier execution
policy rule semantics
storage backend
graph reconstruction engine
memory consolidation engine
Console UX
API/SDK truth
```

## Case Loop

```text
open case
build case_world
attach domains/material/participants
bind attachments to the case
open case_session
derive case_context
bind subjects
bind policy
receive material
evaluate attempts
link receipts
project live state
evaluate case-view quality
close or continue
```

The case is not a workflow engine. Procedure records can exist, but workflow execution is not the core owner.

## SPINE.3R Case-World Rule

```text
Everything that participates in a case enters first as case-world material:
a domain, attachment or binding.
```

Only after attachment and binding can material act as subject, source, actor,
target, projection consumer or carrier participant. Attachment alone does not
grant authority.

## NEW.18B Ref Boundary

`case_ref` is persistent identity. It is not the live case. Runtime operation
uses `case_context` inside a `case_session`; records, receipts and graph edges
continue to persist refs.

## SPINE.4 Observability Rule

The case must remain knowable enough for controlled action. Operational
Observability & Evaluation measures whether the active case view is fresh,
causal, provenanced, coherent, complete, replayable and useful enough for the
subject, model, operator or carrier consuming it.
