# SPINE.51B - CaseHandle / CapabilityLease Boundary

SPINE.51B introduces the first runtime-resolved case boundary before Live
Projection Frame Schema.

Core rule:

```text
refs are identifiers, not authority
bindings are relations, not capabilities
handles are runtime-resolved posture
scopes constrain resolved posture
capability leases are bounded operation permissions
```

## Primitives

- CaseHandle resolves case identity, subject identity, generation posture and
  active runtime status.
- SubjectHandle resolves subject binding within a case.
- AuthorityScope describes what a subject may claim, propose, dispatch,
  execute, review, approve, deny, defer, quarantine, mutate or receipt.
- VisibilityScope describes what may be projected to a subject or consumer.
- ResourceScope describes the resource boundary for filesystem, process,
  provider endpoint or unknown resources.
- CapabilityLease describes bounded permission for one operation family over
  one resource under the current resolved scopes.

AuthorityScope and VisibilityScope are separate. ResourceScope is separate from
authority. A subject can be bound to a case and still have no execution
authority. SubjectHandle does not automatically grant carrier execution.

## Command Surface

```bash
yai case resolve --case <case_ref> --subject <subject_ref>
yai case scope --case <case_ref> --subject <subject_ref>
yai capability derive --case <case_ref> --subject <subject_ref> --operation <operation_family> --resource <resource_ref>
```

`case enter` remains participant admission/projection posture. `case resolve`
is runtime handle/scope resolution. `capability derive` is operation permission
posture.

Expected visible posture includes:

```text
refs_are_authority: false
carrier dispatch allowed
subject_lacks_execute_authority
resource_outside_scope
```

## V0 Semantics

Effective permission is the intersection of subject role, AuthorityScope,
ResourceScope, policy basis and current generation snapshot. It is not a
union-style expansion.

For the filesystem loop case:

- `subject:llm-provider` may claim/propose but cannot execute, approve or
  mutate decisions.
- `subject:operator-reviewer` may review, approve, deny, defer and quarantine.
- `subject:filesystem-sandbox` can read inside sandbox and requires review for
  mutative writes inside sandbox.
- writes outside sandbox are denied with `resource_outside_scope`.

CapabilityLease is not a decision receipt and is not proof of execution. It
only says what a subject may attempt under the current resolved scopes.

## Future Consumers

Live Projection Frame Schema should consume resolved CaseHandle,
SubjectHandle, AuthorityScope, VisibilityScope and ResourceScope in SPINE.52.
It should not reverse-engineer authority from raw refs and summaries.

Future carrier hardening should consume CapabilityLease and ResourceScope
before dispatch. Existing carrier behavior is not globally changed by this
wave.

## Validation

```bash
make check-casehandle-capability-boundary
make smoke-spine51b
```

Expected smoke labels:

```text
case_handle:resolve llm-provider ok
case_handle:resolve filesystem-sandbox ok
case_scope:model no_execute ok
case_scope:reviewer approve ok
capability:model_write denied ok
capability:filesystem_write requires_review ok
capability:filesystem_read minted ok
capability:outside_sandbox denied ok
refs:not_authority ok
```

## Non-Goals

No full Context Compiler. No Live Projection Frame Schema. No provider
execution expansion. No model carrier changes. No new fact classes. No memory
consolidation. No retrieval provider gates. No yai-dev mutation.
