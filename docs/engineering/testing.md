# Testing Doctrine

Tests must falsify the spine. If the core cannot decide, receipt, store, reconstruct or project, tests must expose that.

## Categories

```text
unit
integration
adversarial
conformance
carrier
store
graph
memory
projection
divergence
```

## Primitive Coverage

| Primitive | First proof |
|---|---|
| `case` | open case creates case ref and posture |
| `subject` | bind subject creates case-scoped subject binding |
| `op` | submit attempt targets subject refs |
| `control` | decision record has gates, outcome and basis |
| `effect` | null carrier or observed path produces receipt posture |
| `store` | append persists attempt, decision and receipt records |
| `graph` | reconstruction can relate case, subject, attempt, decision and receipt |
| `memory` | memory is derived only from scoped residue |
| `projection` | query returns controlled read model with provenance |
| `reconcile` | receipt gaps and divergences are visible |

## NEW.1 Minimum Loop

```text
open case
bind subject
submit op attempt
control decision
null carrier / observed receipt
append store record
query projection
```

## NEW.2 Persistent Loop

```text
open case
bind subject
submit op attempt
control decision
receipt
append store record
write JSONL journal
reload journal records
reconstruct subject state
build projection from reloaded records
```

`tests/smoke/persistent-journal/test_persistent_journal.c` proves that the
minimum loop survives outside the process. Generated journal files live under
`build/tmp/` and must remain ignored build artifacts. Smoke tests that write
mutable files must use run-specific directories such as
`build/tmp/new2/persistent-journal-<pid>/` instead of shared journal paths.

## NEW.3 Control Gate Loop

```text
open case
bind subject
submit mutative op attempt
materialize policy rule candidate
evaluate operation gate
build decision basis
emit require_review decision
emit operator_review obligation
emit blocked_receipt requirement
append JSONL records
reload journal records
build control projection
```

`tests/smoke/control-gate/test_control_gate.c` proves that control residue is
persisted and inspectable. The debug commands are:

```text
crates/target/debug/yaictl control summary --journal build/tmp/new3/control-gate-<pid>/journal.jsonl
crates/target/debug/yaictl decision inspect --journal build/tmp/new3/control-gate-<pid>/journal.jsonl
```

## NEW.4 Filesystem Carrier Loop

```text
create sandbox
bind filesystem subject
fs.read read-like attempt
allow decision
filesystem read receipt with hash
fs.write mutative attempt
require_review decision
blocked receipt and no file mutation
manual allow_with_constraints decision
filesystem write receipt with before/after hash
subject state update
filesystem projection
```

`tests/smoke/filesystem-carrier/test_filesystem_carrier.c` proves the first real
effect path while keeping all writes inside a per-run sandbox such as
`build/tmp/new4/filesystem-carrier-<pid>/sandbox/`.

```text
crates/target/debug/yaictl receipt summary --journal build/tmp/new4/filesystem-carrier-<pid>/journal.jsonl
```

## NEW.5 Graph Reconstruction Loop

```text
open case
bind subject
submit operation
emit decision
emit receipt
append graph edges
reload journal
reconstruct receipt chain
build graph projection
```

`tests/smoke/graph-reconstruction/test_graph_reconstruction.c` proves that the
core can explain a receipt through case, operation, decision and subject refs.

```text
crates/target/debug/yaictl graph summary --journal build/tmp/new5/graph-reconstruction-<pid>/journal.jsonl
```

## NEW.6 Operational Memory Loop

```text
open case
bind subject
submit mutative operation
emit require_review decision
emit blocked receipt
append graph edges
reconstruct receipt chain
derive memory candidate
append memory_candidate record
reload journal
build memory projection
```

`tests/smoke/operational-memory/test_operational_memory.c` proves that memory is
derived from scoped residue and persisted as a candidate.

```text
crates/target/debug/yaictl memory summary --journal build/tmp/new6/operational-memory-<pid>/journal.jsonl
```

## Minimum Test Cases

| ID | Scenario | Required Proof |
|---|---|---|
| TC-LOCAL-001 | Agent modifies file. | Attempt, decision, receipt, store record. |
| TC-LOCAL-002 | Dangerous shell command. | Deny/review and no carrier execution. |
| TC-MCP-001 | Tool descriptor drift. | Divergence or review projection. |
| TC-WF-001 | Workflow without receipt. | Missing-receipt divergence. |
| TC-MODEL-001 | Model invocation linked to patch. | Model receipt linked to patch attempt. |
| TC-DB-001 | Database mutation. | Policy gate and receipt. |
| TC-K8S-001 | Rollout observed/interposed. | Receipt or observed divergence. |
| TC-MEM-001 | Memory consolidation scope. | No cross-case recall. |
| TC-AUDIT-001 | Case audit export. | Attempts, decisions, receipts and graph refs. |

## Rule

Tests must not depend on one agent framework. Every failure should produce a divergence, receipt gap, denied decision or explicit unsupported-mode result.
