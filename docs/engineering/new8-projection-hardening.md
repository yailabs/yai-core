# NEW.8 Projection Hardening

NEW.8 turns projection from simple counts into a controlled read-model posture.

Projection is not truth. It is a derived view over residue with an explicit
consumer, projection kind, provenance counts, freshness and redaction posture.

## Inputs

```text
records
decisions
receipts
graph edges
memory candidates
divergences
subject states
```

## New Record Kinds

```text
projection_request
projection_result
```

`projection_request` records who is asking and which view is requested.
`projection_result` records what residue supported the view and which posture
was applied.

## Consumers

```text
operator
model
agent
api
audit
debug
```

## Projection Kinds

```text
operator_summary
model_context
agent_context
audit_packet
debug_dump
control_summary
memory_summary
```

## Redaction Posture

```text
none
summary_only
refs_only
redacted
blocked
```

The first implementation is posture metadata and summary shaping only. It is
not a secret scanner or full redaction engine.

## Freshness

```text
fresh
stale
unknown
```

## Default Policy

```text
operator -> none
model    -> summary_only
agent    -> summary_only
audit    -> none
debug    -> refs_only
```

## Smoke Proof

`tests/smoke/projection-hardening/test_projection_hardening.c` creates residue,
adds operator and model projection request/result records, reloads the journal
and proves that projection inspection sees request/result counts and limited
redaction posture.

```text
yai projection inspect --journal build/tmp/new8/projection-hardening-<pid>/journal.jsonl
```

## Old-YAI Audit

NEW.8 inspected old projection-related material without copying source:

```text
src/substrate/views/*
src/case/surface/*
src/decision/projection/*
src/decision/explanation/*
src/models/frame/*
src/agents/grounding/context_pack.c
src/agents/grounding/live_context.c
```

The useful concepts were provenance, materialization posture, case surface
shape, decision explanation traces and model/context audience boundaries.
Agent context packing remains future `ai-environment` material.
