# Subject Model

A subject is any observable, controllable, invokable, queryable or targetable
entity bound to a case.

Internal Italian form:

```text
Un subject e qualunque entita osservabile, controllabile, invocabile,
interrogabile o targettabile legata a un caso.
```

Without subject, case is only a stream. With subject, case becomes an
operational world.

## 1. Required Terms

```text
subject
source
actor
target
locator
binding
subject state
subject policy
subject observation
subject graph
```

`subject` is not the same as `source`, `actor` or `target`.

A subject can be a source when it contributes material. A subject can be an
actor when it attempts an operation. A subject can be a target when an operation
acts on it.

## 2. Subject Identity

Subject identity is the stable core reference for an operational entity inside a
case. It is not the same as a path, URL, process id or provider name. Those are
locators.

Minimum identity fields:

```text
subject_ref
subject_kind
case_ref
stable_identity_hint
created_at
identity_basis
```

Identity basis records why the core believes the subject is the same entity
across observations. Examples:

```text
repository remote + local root
file inode + path + content hash posture
database connection identity + schema name
model provider + model id
kubernetes uid + namespace + kind + name
```

When identity cannot be proven, the subject must remain `candidate` or become
`conflicted`.

## 3. Subject Locator

A locator tells the system where or how a subject can be observed, invoked or
controlled. A locator can change without changing subject identity.

Locator examples:

```text
filesystem path
repository remote URL
database DSN alias
HTTP endpoint URL
process pid
model provider route
MCP tool name
kubernetes api object ref
document URI
```

Locator rules:

```text
locators are not primary identity
locators require provenance
locators can be stale
multiple locators can point to one subject
one locator can later resolve to a different subject
```

## 4. Subject Lifecycle

```text
discovered
candidate
bound
observed
controlled
retired
lost
conflicted
```

Lifecycle meanings:

| State | Meaning |
|---|---|
| `discovered` | material suggests an entity exists |
| `candidate` | entity is recognized but not yet case-bound |
| `bound` | entity has a case binding and subject reference |
| `observed` | YAI has recorded state or activity for the subject |
| `controlled` | at least one operation path is gated or carrier-owned |
| `retired` | binding is intentionally ended |
| `lost` | locator no longer resolves or subject cannot be observed |
| `conflicted` | identity, locator or state evidence disagrees |

Lifecycle transitions must be recorded. A subject can be lost without being
retired, and conflicted without being removed.

## 5. Subject Binding Modes

```text
attached
observed
interposed
carrier_owned
embedded
external
```

| Binding mode | Meaning |
|---|---|
| `attached` | known to case, but no control claim |
| `observed` | activity/state is observed and recorded |
| `interposed` | attempts pass through a YAI decision point before external effect |
| `carrier_owned` | YAI carrier executes or blocks the effect |
| `embedded` | host system calls YAI inside its own control boundary |
| `external` | represented through imported claims or receipts |

Binding mode describes relationship to the case. It does not by itself imply
enforcement strength; control mode carries that guarantee.

## 6. Subject Control Modes

```text
observed
advisory
interposed
enforced
carrier_owned
embedded
```

| Control mode | Core guarantee |
|---|---|
| `observed` | YAI can record and reconstruct observations |
| `advisory` | YAI can recommend decisions without blocking |
| `interposed` | YAI decides before forwarding an effect request |
| `enforced` | host or carrier is expected to honor a deny/constraint |
| `carrier_owned` | YAI carrier performs or blocks the effect |
| `embedded` | host embeds YAI decision and receipt protocol |

Control mode must be reflected in receipts and projections. A subject in
observed control mode must not be described as blocked or enforced.

## 7. Binding Modes Vs Control Modes

Binding mode and control mode are related but not identical.

| Binding mode | Allowed control modes | Notes |
|---|---|---|
| `attached` | `observed`, `advisory` | Known to the case, with no enforcement claim. |
| `observed` | `observed`, `advisory` | Activity/state can be recorded and reconstructed. |
| `interposed` | `interposed`, `enforced` | YAI decides before an external effect path proceeds. |
| `carrier_owned` | `carrier_owned`, `enforced` | YAI carrier owns the effect path and receipt. |
| `embedded` | `embedded`, `enforced` | Host calls YAI inside a declared control boundary. |
| `external` | `observed`, `advisory` | Represented through imported claims or receipts only. |

`embedded` means the host integrates YAI into its own control path. `external`
means YAI observes or imports residue from outside its control path. Neither
term should be used as a vague trust label.

## 8. Initial Subject Kinds

```text
file
directory
repository
database
table
endpoint
socket
process
service
model
provider
agent
tool
workflow
kubernetes_object
document
dataset
sensor
external_system
```

Subject kinds are classification hints. They do not replace policy, carrier or
receipt requirements.

## 9. Binding Fields

```text
subject_ref
case_ref
subject_kind
locator
binding_mode
control_mode
trust_class
observability
allowed_effects
forbidden_effects
policy_refs
carrier_refs
receipt_obligations
projection_permissions
current_state_ref
last_observed_at
```

Binding records must be append-friendly. Updates should create new binding or
state records instead of silently mutating old history.

## 10. Subject State Model

Subject state is the latest known posture of a subject in a case. It is not the
entire external truth of the object.

Minimum state fields:

```text
state_ref
subject_ref
case_ref
state_kind
observed_at
observed_by
locator_snapshot
content_or_status_hash
capability_posture
policy_posture
receipt_refs
evidence_refs
freshness
confidence
```

State kinds can include:

```text
exists
missing
changed
reachable
unreachable
running
stopped
healthy
degraded
unknown
conflicted
```

State is always time-bound. Stale state must be marked stale instead of reused
as current truth.

### NEW.2 Subject State V0

NEW.2 implements a smaller C state object for reconstruction from persisted
records:

```text
subject_ref
case_ref
state_kind
last_receipt_id
last_decision_id
last_observed_at
summary
```

Initial state kinds:

```text
bound
observed
controlled
lost
conflicted
unknown
```

Only `bound` and `observed` are exercised in NEW.2. The reconstruction rules
are:

```text
subject_binding record -> state_kind = bound
receipt record         -> state_kind = observed, last_receipt_id updated
```

This state is still residue-based. It is the latest YAI-known posture from the
journal, not complete external truth about the subject.

## 11. Subject Policy Scope

Policy can attach to:

```text
case
subject kind
subject instance
actor subject
target subject
source subject
carrier
operation class
projection
retention posture
```

Subject policy binding answers: what can this subject do, what can be done to
it, what receipts are required, what projections are allowed and what memory can
be derived.

Subject policy must not be stored only as text. It must resolve to gates,
constraints, obligations, receipt requirements or projection rules.

## 12. Subject Observation Model

An observation is a recorded fact or claim about a subject at a time. It can
come from ingest, carrier execution, external receipt import, daemon probe,
operator command or adapter.

Minimum observation fields:

```text
observation_ref
subject_ref
case_ref
source_ref
observation_kind
observed_at
material_ref
state_delta_ref
confidence
provenance
```

Observation does not imply control. It provides residue for store, graph,
memory, projection and reconcile.

## 13. Subject Graph

The subject graph connects subjects to cases, operations, decisions, receipts,
policies and other subjects.

Core edge types:

```text
case_binds_subject
subject_observed_by_source
subject_acted_as_actor
op_targets_subject
op_reads_subject
carrier_acts_on_subject
receipt_updates_subject
policy_applies_to_subject
subject_depends_on_subject
memory_refs_subject
projection_includes_subject
```

The graph is not optional decoration. It is how the system reconstructs
operational worlds across time.

## 14. Concrete Examples

| Subject | Identity basis | Locator | Typical ops | Receipt expectation |
|---|---|---|---|---|
| repository | local root + remote | path + remote URL | read, diff, commit, patch | patch or VCS receipt |
| file | path + hash posture | filesystem path | read, write, delete | content hash/state receipt |
| process | pid + command + parent | pid/proc entry | spawn, signal, inspect | exit/status receipt |
| endpoint | service identity + URL | HTTP URL | request, mutate, probe | HTTP response/probe receipt |
| database | DSN alias + schema | connection alias | query, mutate, export | query/mutation receipt |
| model | provider + model id | provider route | invoke, embed, classify | model invocation receipt |
| agent | agent id + runtime binding | process/tool endpoint | act, request, delegate | attempt/response receipt |
| workflow | workflow id + run id | workflow system ref | start, approve, cancel | run event receipt |
| kubernetes object | uid + namespace/kind/name | API object ref | apply, patch, rollout | admission/event receipt |
| external system | system id + adapter | adapter endpoint | import, observe, request | external receipt |

## 15. Model Subject Doctrine

A model is not inside the core. A model becomes meaningful to YAI as a
case-bound subject with locality, provider, posture and observed behavior.

Model subject posture includes:

```text
model identity
provider/runtime
locality
declared provider policy
observed behavior
projection posture
redaction posture
review posture
receipt posture
```

Local model:

```text
more controllable runtime
better privacy
not automatically reliable
```

Remote closed model:

```text
unknown internal policy
provider-declared policy
observed behavior only
projection should default stricter
```

L0 provider scouting can begin immediately outside the core. It can produce
notes and fixtures, but the first canonical naked model case experiment is
NEW.26. Core-owned invocation waits for NEW.28 model carrier v0.

Agent frameworks are later subjects, actors or sources. They do not own the
core subject model.

## 16. Subject Non-Goals

```text
subject is not a universal object database
subject is not a replacement for provider registries
subject is not a hidden agent identity system
subject is not source material by itself
subject identity is not the same as locator
subject state is not complete external truth
```

The subject model exists so the case can govern a real operational world without
claiming ownership of everything outside YAI.
