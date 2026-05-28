# Next Wave Recommendation

Recommended next wave:

```text
SPINE.51B - CaseHandle / CapabilityLease Boundary
```

Purpose:

```text
Introduce runtime-resolved handles and capability leases before Live Projection
Frame Schema, so projections are compiled from resolved authority/visibility
rather than raw refs.
```

Why this should precede SPINE.52:

- `case_ref` and `subject_ref` are identifiers, not authority.
- Current `authority_scope` is split between records, summaries and partial
  enforcement.
- Filesystem sandbox enforcement exists, but it is not generalized as
  ResourceScope.
- Provider/model refs are visible in labs and runtime surfaces, but they do not
  create a CapabilityLease.
- Live Projection Frame Schema should consume CaseHandle, SubjectHandle,
  AuthorityScope, VisibilityScope and ResourceScope posture rather than
  reverse-engineering semantics from raw refs and summaries.

Minimum SPINE.51B shape:

```text
CaseHandle: resolved case identity, generation snapshot, active case/session posture
SubjectHandle: resolved subject binding within a case
AuthorityScope: what may decide/approve/execute/display
VisibilityScope: what may be projected to model/operator/audit
ResourceScope: filesystem/process/provider target boundary
CapabilityLease: bounded permission for a carrier/provider operation
```

Non-goals for SPINE.51B should remain narrow:

```text
No full Context Compiler.
No provider execution expansion.
No new fact classes.
No memory consolidation.
No yai-dev mutation.
```
