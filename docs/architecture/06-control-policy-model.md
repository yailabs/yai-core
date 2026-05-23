# Control And Policy Model

`control` is the machine plane that turns policy into decisions, obligations and
receipt requirements.

`decision` is not a root. It is an object inside `control`.

## 1. Materialization Chain

```text
policy source
-> policy material
-> policy claim
-> policy rule
-> case policy binding
-> subject policy binding
-> gate
-> decision
-> obligation
-> receipt requirement
-> policy memory
```

Canonical rule:

```text
A policy is not enforceable until it becomes a machine gate, constraint,
obligation, receipt requirement, projection rule, retention rule or failure mode.
```

Internal Italian form:

```text
Una policy non e enforceable finche non diventa gate macchina, vincolo,
obligation, requisito di receipt, regola di projection, regola di retention
o failure mode.
```

## 2. Policy Lifecycle

```text
imported
classified
claimed
normalized
bound
active
conflicting
superseded
expired
retired
```

| State | Meaning |
|---|---|
| `imported` | policy source material entered the system |
| `classified` | material has type, source and scope hints |
| `claimed` | enforceable or advisory claims were extracted |
| `normalized` | claims became rule candidates |
| `bound` | rules are attached to case, subject, op, carrier or projection scope |
| `active` | rules can participate in gate evaluation |
| `conflicting` | rules disagree and require precedence or review |
| `superseded` | newer policy replaces older policy |
| `expired` | policy no longer applies by time or condition |
| `retired` | policy is intentionally removed from active evaluation |

Lifecycle transitions must be stored. A retired or expired policy can still be
evidence for historical decisions.

## 3. Policy Object Model

Minimum policy objects:

```text
policy_source
policy_material
policy_claim
policy_rule
policy_binding
policy_conflict
policy_precedence
policy_obligation
gate
admission
decision
outcome
review_requirement
evidence_requirement
```

Minimum rule fields:

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

Policy effects:

```text
allow
deny
constrain
require_review
require_evidence
require_redaction
require_receipt
require_retention
observe_only
```

Policy text is evidence, not enforcement. Enforcement begins only at the rule,
gate, constraint, obligation or requirement layer.

## 3A. Model Policy Materialization

For model/provider work, policy is materialized twice:

```text
cognitively through projection
operationally through gates, decision and carrier
```

The model projection may include allowed actions, forbidden actions, review
requirements, evidence requirements, policy summaries, memory candidates and
redaction posture. This helps shape model behavior but does not replace gate
evaluation.

Model output is imported as a claim, observation or proposed operation. Control
evaluates proposed operations before any carrier path can execute. L0 provider
scouting can begin immediately outside the core, but the first canonical naked
model case experiment is SPINE.35 and the first core-owned model invocation is
SPINE.37.

Agent frameworks are not first. They enter later as external subjects, actors
or sources after naked model behavior is measurable.

## 4. Policy Applicability

Applicability answers whether a rule should participate in a gate evaluation.

Applicability can depend on:

```text
case_ref
case posture
actor_subject_ref
target_subject_refs
subject kind
subject state
operation effect_class
sensitivity
locality
mutability
carrier_ref
integration mode
projection consumer
time window
```

If applicability cannot be computed, the gate must produce either `defer`,
`require_review`, `require_evidence` or a configured failure mode. It must not
silently allow.

Canonical failure modes are defined in section 13 and in
[policy-materialization.md](../protocols/policy-materialization.md).

## 5. Subject Policy Binding

Subject policy binding attaches policy to a subject role and control posture.

Binding answers:

```text
what this subject may do
what may be done to this subject
which carrier may act on it
which receipts are required
which projections may include it
which memory may derive from it
which retention rules apply
```

Subject policy is evaluated together with case policy. Subject-level allow does
not override case-level deny unless explicit precedence says so.

## 6. Gate Evaluation Order

Default gate order:

```text
case gate
subject gate
actor gate
operation gate
carrier gate
projection gate
retention gate
```

Gate meanings:

| Gate | Purpose |
|---|---|
| case gate | is the case open, scoped and in a posture that admits this attempt |
| subject gate | are target/source subjects bound and in acceptable state |
| actor gate | may this actor subject attempt this class of operation |
| operation gate | does effect_class/sensitivity/locality/mutability pass policy |
| carrier gate | may this carrier execute or observe the attempt |
| projection gate | what can be shown to the requested consumer |
| retention gate | what must be retained, redacted, pinned or decayed |

Gate order can be optimized, but the decision record must preserve the logical
evaluation order and the result for each participating gate.

## 7. Obligation Model

An obligation is a required follow-up created by a decision or policy rule.

Obligation examples:

```text
require receipt
require operator review
require evidence attachment
require redaction before projection
require retention pin
require follow-up observation
require compensation attempt
require incident marker
```

## NEW.3 Control Gate V0

NEW.3 implements the first C control gate skeleton. It is intentionally smaller
than the full model above.

Implemented objects:

```text
policy_rule
gate_result
decision_basis
obligation
receipt_requirement
failure_mode
```

Implemented decision logic:

```text
case missing          -> deny
target subject missing -> deny
mutative operation    -> require_review
read-like operation   -> allow
```

The mutative path uses:

```text
policy rule: mutative_operation_requires_review
gate result: operation require_review
obligation: operator_review
receipt requirement: blocked_receipt
```

NEW.3 does not ingest policy text, resolve policy conflicts, run operator review
flows or enforce real carriers. It only creates machine-shaped control residue
that can be persisted, reloaded, projected and inspected.

Minimum obligation fields:

```text
obligation_ref
decision_ref
policy_ref
obligation_kind
subject_refs
due_condition
status
evidence_refs
created_at
fulfilled_at
```

Obligations must be visible to store, graph, projection and reconcile.

## 8. Policy Conflict And Precedence

A conflict exists when applicable rules produce incompatible effects or
requirements.

Conflict examples:

```text
one rule allows while another denies
one rule requires projection while another requires redaction
one rule requires retention while another requires deletion
one subject policy permits a carrier while case policy forbids it
```

Precedence can be based on:

```text
explicit priority
policy source authority
case binding specificity
subject specificity
deny-over-allow default
freshness
operator review
```

Default posture is conservative: unresolved conflict should produce `defer`,
`require_review` or `deny`, depending on configured failure mode.

## 9. Decision Outcomes

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

`allow` admits an attempt under the current constraints. `deny` rejects it.
`observe_only` records without enforcement claim. `allow_with_constraints`
creates constraints and obligations that must travel with the carrier request or
projection.

## 10. Decision Basis

Decision basis is the machine explanation for an outcome. It is not marketing
copy and not a natural language justification alone.

Decision basis includes:

```text
applicable policy refs
gate result refs
subject state refs
case posture
attempt fields used
evidence refs
conflict refs
precedence rule refs
obligation refs
receipt requirement refs
```

Projection may render the basis in human text, but the stored basis must remain
structured.

## 11. Decision Record Shape

Minimum decision record:

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

Recommended additional fields:

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

A decision record must not claim execution. Execution or observation belongs to
effect receipts.

## 12. Receipt Requirements

Receipt requirements define what evidence must return after a decision.

Examples:

```text
carrier receipt required
external receipt accepted
post-state observation required
hash delta required
exit status required
model invocation metadata required
operator acknowledgement required
```

Missing receipt requirements produce reconciliation work. They are not optional
logging hints.

## 13. Failure Modes

Failure modes must be explicit:

```text
fail_closed
fail_open_observed
require_review
require_evidence
defer_until_policy_available
quarantine_subject
deny_without_receipt_path
```

No policy, stale subject state, unknown carrier, unresolved conflict and missing
receipt path are all machine conditions, not comments.

## 14. Policy Memory

Policy memory derives from decisions, conflicts, reviews, receipts and
reconciliation outcomes.

Policy memory can remember:

```text
which policy caused repeated denials
which subjects frequently require review
which carriers often miss receipts
which conflicts were resolved by operator review
which projection rules caused redaction
```

Policy memory must remain scoped. It can inform future projection or review, but
it cannot silently override active policy rules.

## 15. Core Rule

Policy applies to case, subject, operation, carrier, projection, retention and
receipt posture. `decision` stays inside `control`; `control` is the root that
turns policy into machine behavior.
