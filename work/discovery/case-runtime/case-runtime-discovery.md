# SPINE.51A - Case Runtime Discovery / Ref-Handle-Capability Audit

Status: discovery complete for current `yai` and read-only `yai-dev` surfaces.

Scope:

```text
yai: include/, system/, engine/yai-engine, cmd/yai, tests/smoke, labs/filesystem-loop, labs/context-residency
yai-dev: ../yai-dev/src/case, runtime, decision, substrate, models, capabilities, policy-like residue
```

No implementation in this wave. Do not mutate yai-dev.

`labs/model-behavior/` is not present in this worktree. Model behavior evidence
currently appears through `tests/smoke/model-behavior-policy-facts` and the
context-residency/provider surfaces.

## Core Finding

Today a case in `yai` is mostly a stable `case_ref` carried by records, journal
entries, LMDB records, projections, facts and graph relations. It is also
partially materialized through case-domain, case-attachment, case-binding,
subject-binding, case-session and hot-state surfaces.

It is not yet a resolved runtime authority object. There is no CaseHandle,
SubjectHandle, CapabilityLease, ResourceScope, VisibilityScope or formal
AuthorityScope object. Existing `_ref` and `_id` fields identify and correlate
records; they must not imply authority.

## Main Questions

1. What is a case today in `yai`?

`case_ref` is defined in `include/yai/case/case_ref.h` and initialized by
`system/case/case_ref.c`. Runtime loops create `case:new12-filesystem` and
write `case`, `case_domain`, `case_attachment`, `case_binding`,
`subject_binding`, policy, authority, attempt, decision, receipt, projection,
memory and graph records under it. `engine/yai-engine/src/record.rs` stores the
same case ref in normalized record envelopes.

2. Where is `case_ref` resolved?

It is not resolved into a handle. It is validated as non-empty in C record,
gate, projection and context functions, and selected from CLI args or fixture
defaults in daemon/client code. `case_session_open` materializes counts and hot
state from a journal, but does not produce a durable CaseHandle.

3. Where is `subject_ref` bound to a case?

`include/yai/subject/subject_binding.h` and `system/subject/subject_binding.c`
define a subject binding object with binding/control modes. The filesystem
loop emits `subject_binding` records for filesystem, model provider, terminal
and policy-pack subjects. `case_attachment` and `case_binding` records also
declare relation-like bindings.

4. Where is authority computed?

Policy/gate/decision logic exists in `system/control/gate.c`,
`system/control/decision.c` and `system/control/policy_rule.c`. Runtime
fixtures emit `authority_scope` records and response text like
`model:interpretation_only terminal:display_only`. Review commands print
`review_authority_subject: subject:operator-reviewer` and `authority_scope:
local-dev`. This is partial: authority is partly enforced by gates and carrier
checks, partly displayed in records/summaries.

5. Where is visibility computed?

Projection counting and redaction posture live in `system/projection/` and
`engine/yai-engine/src/projection.rs`. Records such as `projection_rule`,
`projection_request`, `projection_result` and `participant_view_frame` express
visibility intent. Model context is currently represented as summary-only
projection records, not as a formal VisibilityScope.

6. Where is resource scope enforced?

Filesystem sandbox enforcement exists in
`system/effect/carriers/filesystem_carrier.c` through
`yai_filesystem_path_is_inside_sandbox`. Review resolution in `cmd/yai/src/main.rs`
also refuses an approved target outside the sandbox. Process scope is limited
to test-owned PID smoke surfaces. Provider endpoint/LAN scope is currently
registry/status/dry-run evidence, not a ResourceScope object.

7. Where is policy applied?

Policy is applied by `yai_policy_rule_for_attempt`, `yai_gate_evaluate_attempt`
and `yai_control_decide`. Filesystem-loop records include `policy_rule` and
`projection_rule`. In review paths, policy appears as reason fields and pending
operator state.

8. Where is operation gating performed?

The C gate path evaluates attempts into gate results and decisions. Review-loop
paths in `cmd/yai/src/main.rs` create `review_request` and `control_pending`
records for a mutative filesystem write requiring operator resolution.

9. Where are review/defer/quarantine states created?

`cmd/yai/src/main.rs` creates review fixture records and implements
`yai control approve|deny|defer|quarantine`. `review_request`,
`review_decision` and `control_pending` are Rust record kinds. Approve writes a
filesystem dispatch and executed receipt; deny/defer/quarantine preserve
`carrier_attempted: false` and `execution_performed: false`.

10. Where do carriers receive decision/capability material?

The filesystem carrier receives an `attempt`, `decision`, sandbox root and path.
It enforces the decision outcome and sandbox path. There is no CapabilityLease.
Carrier outcome harnesses simulate carrier posture across families; skeleton
families do not execute.

11. Where do receipts prove execution/non-execution?

Effect/filesystem receipts carry `case_ref`, `attempt_id`, `decision_id`,
`receipt_id`, target subject, carrier/effect kind, status, path/hash/summary.
Review resolution records include receipt summaries for executed, blocked,
deferred and quarantined states. Facts and graph relations derive from these
records but are not proof authority themselves.

12. Where does `yai-dev` still contain stronger/older ideas?

`yai-dev` contains richer case creation, subject permissions, case policy
binding/effective policy, capability routing/admissibility, capability records,
runtime boundary guards, runtime access/auth, provider binding, model provider
selection, mediation/authority gates and controlled action/enforcement
material. These are concept sources, not code to import wholesale.

13. What can be deleted from `yai-dev` later?

Legacy drains, duplicate surfaces, GUI/audio/runtime presentation registries,
old substrate view drains and old surface/CLI shims can be left archived or
deleted later after concepts are either extracted or superseded in `yai`.

14. What must be extracted before `yai-dev` is emptied?

Extract the minimal semantics for CaseHandle/SubjectHandle, CapabilityLease,
ResourceScope, VisibilityScope, AuthorityScope, case generation snapshot,
provider/model binding, review authority and capability guard/admissibility.

## Ref Surfaces

See `ref-binding-authority-map.tsv` for the full table.

Summary:

```text
case_ref: identity/correlation only; future CaseHandle needed
subject_ref: identity/correlation only; future SubjectHandle needed
attempt_id: operation attempt identity; no authority
decision_id: decision identity; authority only through resolved decision object
receipt_id: receipt identity; proof reference, not proof by itself
record_id: durable record identity
review_id: review workflow identity; local-dev reviewer authority is displayed
provider_ref/model_ref: provider/model identity in summaries; no lease
projection_id/frame_id/thread_id: projection/session identifiers; no visibility handle
```

## Binding Surfaces

`case_binding`, `subject_binding`, `case_attachment`, provider attachment-like
records, `case_domain`, `policy_subject` and reviewer subject are declaration
surfaces today. They create relation evidence and projection inputs. They do
not create executable capability by themselves.

## Authority Surfaces

Current authority is split:

```text
enforced: gate/decision checks; filesystem sandbox path check; review approve-only execution
displayed: authority_scope records; daemon response authority_scope; review authority text
derived: facts/report sections; graph relations; projection counts
missing: resolved AuthorityScope object and CapabilityLease
```

## Resource Scope Surfaces

Filesystem sandbox is the strongest current resource scope. Process scope is
test-owned PID only. Provider endpoint/LAN target exists as runtime/provider
status and dry-run material. Network/database/git scopes are carrier skeletons.

## Gate / Decision Cycle

Actual current algorithm:

```text
attempt -> gate -> decision -> dispatch/carrier -> receipt -> divergence/fact/graph relation
```

Implemented portions:

```text
attempt: record and C struct
gate: C gate result plus review-loop summary records
decision: C decision and record summaries
dispatch/carrier: filesystem active minimal, process limited, others skeleton
receipt: receipt/effect/filesystem receipt records
divergence: record kind and carrier consistency smoke/facts; no reconcile action
fact: DuckDB derived facts frozen in SPINE.51
graph relation: materialized/rebuilt relation records and RuntimeGraph diagnostics
```

## Review Loop

Review request and control pending are Rust record kinds and CLI-visible
records. Commands are:

```bash
yai control pending --case <case_ref>
yai control show <review_id>
yai control approve <review_id> --reason <reason>
yai control deny <review_id> --reason <reason>
yai control defer <review_id> --reason <reason>
yai control quarantine <review_id> --reason <reason>
```

Approve performs the filesystem write and emits executed receipt posture.
Deny/defer/quarantine do not attempt the carrier and do not perform execution.

## Projection / Model Visibility

`projection_result` records distinguish operator/model consumers and redaction
such as `summary_only`. Prompt/model paths use participant view frames and
preview-only transcript retention. This is visibility posture, not a
VisibilityScope. Raw journal access remains an operator/debug surface and
should be excluded from future model visibility by resolved scope.

## Classification Summary

```text
already_in_yai: refs, record kinds, subject binding, case domain/attachment/binding records, gate/decision, filesystem sandbox check, review records/CLI, receipts, projection counts, graph/fact derivations
partially_in_yai: authority_scope, visibility, resource scope, provider/model binding, review authority
candidate_to_extract_from_yai_dev: case identity lifecycle, subject permissions, capability routing/guards, runtime controlled action, provider/model binding, policy effective binding
future_case_handle: resolved case/session/generation snapshot
future_capability_lease: operation-carrier-provider lease
future_resource_scope: filesystem/process/provider/network/database/git scope object
future_visibility_scope: model/operator/audit projection visibility
future_authority_scope: authority separate from visibility and identity
future_projection_boundary: compile projections from resolved handles/scopes
future_carrier_boundary: carriers accept leases/scopes, not raw refs alone
obsolete_yai_dev/duplicate_yai_dev/leave_archived: old drains and presentation surfaces after concept extraction
```
