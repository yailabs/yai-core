# Control Runtime

YAI treats control as an operational loop, not a label.

SPINE.49 adds analytical carrier, divergence and memory quality facts without
changing control behavior. carrier facts measure carrier posture; divergence
facts are not reconcile action; memory facts are not memory. Facts are not
truth and cannot authorize execution.

```text
attempt -> gate -> decision -> dispatch -> carrier -> receipt
```

If execution cannot proceed safely, the control path records the posture and
keeps the carrier from running.

## Control Concepts

- Gate: evaluates the attempted operation against case material and policy.
- Decision: records allow, deny, require-review, defer, quarantine or related
  posture.
- Dispatch: chooses whether a carrier may be attempted.
- Carrier: the boundary-specific executor or observer.
- Receipt: evidence of execution, block, defer, quarantine or divergence.
- Divergence: structured mismatch between intended, observed and recorded
  state.

## Review Loop

`require_review`, `deferred` and `quarantined` are active control states.

- `pending_operator` means a review request is waiting for operator authority.
- `approve` may produce an allow-with-constraints decision and dispatch a safe
  carrier.
- `deny` is final and creates a blocked receipt.
- `defer` keeps execution pending a condition.
- `quarantine` isolates the operation and prevents execution.

For deny, defer and quarantine, `carrier_attempted: false` and
`execution_performed: false` remain visible. `subject:linenoise-terminal` is
the prompt surface only. `subject:operator-reviewer` is the local-dev review
authority.

## CaseHandle / CapabilityLease Boundary

SPINE.51B adds runtime-resolved inspection before future carrier hardening.
refs are identifiers, not authority. bindings are relations, not capabilities.

AuthorityScope and VisibilityScope are separate scopes. ResourceScope is
separate from authority. A CapabilityLease is a bounded operation permission,
not a decision receipt and not proof of execution.

The control path for capability derivation is:

```text
CaseHandle + SubjectHandle
-> AuthorityScope + VisibilityScope + ResourceScope
-> CapabilityLease
-> carrier dispatch allowed posture
```

For the filesystem loop, `subject:llm-provider` receives
`subject_lacks_execute_authority` for `filesystem.write`. The filesystem
sandbox subject receives `requires_review` for inside-sandbox writes, `minted`
for inside-sandbox reads and `resource_outside_scope` for outside-sandbox
writes.

The CLI surfaces are `case resolve`, `case scope` and `capability derive`.
