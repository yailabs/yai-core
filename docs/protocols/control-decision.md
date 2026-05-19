# Control Decision Protocol

Purpose: record the outcome of applying control to an attempted operation.

Decision is an object inside `control`, not a root. This protocol must remain
aligned with [06-control-policy-model.md](../architecture/06-control-policy-model.md).

## Inputs

```text
case_ref
attempt_ref
actor_subject_ref
target_subject_refs
policy_refs
gate_results
evidence_refs
projection_context_ref optional
```

## Decision Record Shape

```text
decision_id
case_ref
attempt_ref
actor_subject_ref
target_subject_refs
policy_refs
gate_results
outcome
constraints
obligations
evidence_refs
receipt_requirements
issued_at
```

Recommended optional fields:

```text
decision_version
control_mode
integration_mode
conflict_refs
precedence_refs
basis_ref
review_ref
expires_at
```

## Outcomes

```text
allow
deny
defer
observe_only
require_review
require_evidence
require_redaction
allow_with_constraints
```

## Failure Modes

```text
fail_closed
fail_open_observed
require_review
require_evidence
defer_until_policy_available
quarantine_subject
deny_without_receipt_path
```

## Rule

A decision record must not claim execution. Execution, observation or imported
external effect truth belongs to receipts.
