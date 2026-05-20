# Policy Materialization Protocol

Purpose: transform policy material into machine-enforceable control artifacts.

This protocol follows
[06-control-policy-model.md](../architecture/06-control-policy-model.md).

## Chain

```text
policy_source
-> policy_material
-> policy_claim
-> policy_rule
-> case_policy_binding
-> subject_policy_binding
-> gate
-> decision
-> obligation
-> receipt_requirement
-> policy_memory
```

## Rule

Policy text is not enforcement. Enforcement begins only when policy becomes a
gate, constraint, obligation, receipt requirement, projection rule, retention
rule or failure mode.

Unresolved applicability must not silently allow. It must produce a configured
failure mode.

## Canonical Failure Modes

```text
fail_closed
fail_open_observed
require_review
require_evidence
defer_until_policy_available
quarantine_subject
deny_without_receipt_path
```

## Minimum Policy Rule Fields

```text
policy_ref
rule_ref
source_ref
claim_ref
rule_kind
scope
applicability
effect
precedence
valid_from
valid_until
failure_mode
evidence_refs
```

## NEW.3 Rule Candidate V0

NEW.3 does not ingest external policy sources. It creates a local policy rule
candidate directly from an operation attempt:

```text
mutative_operation -> require_review
read_like_operation -> allow
missing_subject -> deny
```

The rule candidate feeds a gate result and decision basis. It is persisted as a
`policy_rule` store record so later waves can replace the candidate generator
with real policy materialization without changing the journal proof.
