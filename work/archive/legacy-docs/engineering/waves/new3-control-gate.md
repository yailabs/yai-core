Historical/superseded engineering record. Not an active source of truth.

# NEW.3 Control Gate

NEW.3 turns `control` from a compact decision helper into the first structured
gate machine. It still does not implement a full policy engine or real carrier
enforcement.

## Implemented Surface

```text
policy rule v0
gate result v0
decision basis v0
obligation v0
receipt requirement v0
failure mode enum
control record persistence
control projection counts
yai control inspection
```

## Object Shapes

`policy_rule` records the rule candidate used by the gate:

```text
rule_id
case_ref
scope_kind
effect_class
condition_kind
effect
failure_mode
summary
```

`gate_result` records the machine gate result:

```text
gate_id
case_ref
attempt_id
rule_id
gate_kind
result
reason
```

`decision_basis` links the decision to its rule and gate:

```text
basis_id
case_ref
attempt_id
gate_id
rule_id
gate_count
policy_rule_count
evidence_count
summary
```

`obligation` and `receipt_requirement` record follow-up work required by the
decision.

## Decision Logic V0

```text
missing case or subject -> deny / block
mutative operation      -> require_review
read-like operation     -> allow
```

For the mutative path NEW.3 emits:

```text
rule: mutative_operation_requires_review
gate: operation require_review
obligation: operator_review
receipt_requirement: blocked_receipt
```

The decision record references the basis, gate, rule, obligation and receipt
requirement. It does not claim execution; effect truth remains in receipts.

## Persistence

NEW.3 reuses the NEW.2 JSONL store record format and adds record kinds:

```text
policy_rule
gate_result
decision_basis
obligation
receipt_requirement
```

The smoke path writes:

```text
build/tmp/new3/journal.jsonl
```

This is a generated dev/test path and is not a stable user store layout.

## Projection And yai

Control projection summarizes:

```text
records
decisions
rules
gates
obligations
receipt_requirements
```

Debug commands:

```text
yai control summary --journal build/tmp/new3/journal.jsonl
yai decision inspect --journal build/tmp/new3/journal.jsonl
```

## Out Of Scope

```text
no full policy language
no normative source ingestion
no policy conflict engine
no operator review flow
no real carrier enforcement
no filesystem/process/model carrier
no graph/index/memory engine
no daemon IPC
no Rust production backend
```

## Validation

```text
make smoke-new3
make smoke
make check
cargo fmt --check
git diff --check
```
