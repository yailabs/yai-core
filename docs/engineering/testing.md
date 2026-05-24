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
query
rust engine
daemon ipc
daemon loop
hot state
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
| `query` | journal residue can be filtered without becoming a database |
| `rust engine` | Rust can consume C journal residue through FFI |
| `daemon ipc` | `yai` can reach resident `yaid` over local IPC |
| `daemon loop` | `yaid` can serve bounded core loops over local IPC |
| `hot state` | active case/session/projection freshness is visible without treating journal as the live surface |

## SPINE.23 Hot State Loop

```text
initialize hot state
bind active case/session/context
mark projection fresh
mark decision and receipt
invalidate projection with stale reason
refresh projection
write hot-state snapshot
```

`tests/smoke/hot-state/test_hot_state.c` proves the v0 cache boundary and
snapshot path.

```text
make smoke-spine23
yai hot status
```

## SPINE.24 Hot State Snapshot Loop

```text
missing snapshot returns unavailable/missing_snapshot
corrupt snapshot returns unavailable/invalid_snapshot
daemon writes yai.hot_state.v1 snapshot
yai hot status reports active snapshot
yai doctor reports path, status, schema and readability
no hot-state.json.tmp remains after write
```

`tests/smoke/hot-state-snapshot/test_hot_state_snapshot.sh` proves snapshot
lifecycle behavior through the CLI.

Manual command checks:

```text
target/debug/yai doctor
target/debug/yai hot status
target/debug/yai daemon run-minimum-loop --socket <socket>
target/debug/yai hot status
```

## SPINE.25 Hot State Session/Context Loop

```text
initialize hot state
activate case_session
mark case_world loaded
activate case_context
select interaction thread
build participant view frame
mark decision and receipt stale projection
refresh projection to fresh
copy snapshot with session/context fields
```

`tests/smoke/hot-state-session/test_hot_state_session.c` proves that the
runtime cache carries session, world, context, active thread and participant
view fields without becoming durable truth.

```text
make smoke-spine25
yai hot status
```

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
yai control summary --journal build/tmp/new3/control-gate-<pid>/journal.jsonl
yai decision inspect --journal build/tmp/new3/control-gate-<pid>/journal.jsonl
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
yai receipt summary --journal build/tmp/new4/filesystem-carrier-<pid>/journal.jsonl
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
yai graph summary --journal build/tmp/new5/graph-reconstruction-<pid>/journal.jsonl
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
yai memory summary --journal build/tmp/new6/operational-memory-<pid>/journal.jsonl
```

## NEW.7 Reconcile / Divergence Loop

```text
create conflicting residue
detect denied_but_executed
append divergence record
append reconciliation record
create receipt_without_decision residue
append second divergence and reconciliation records
reload journal
build reconcile projection
```

`tests/smoke/reconcile-divergence/test_reconcile_divergence.c` proves that
mismatch is persisted as explicit residue instead of hidden by projection.

```text
yai reconcile summary --journal build/tmp/new7/reconcile-divergence-<pid>/journal.jsonl
```

## NEW.8 Projection Hardening Loop

```text
create residue
append memory candidate
append divergence
create operator projection request/result
create model projection request/result
reload journal
inspect projection request/result counts
verify redaction posture
```

`tests/smoke/projection-hardening/test_projection_hardening.c` proves that
projection records are persisted with consumer, provenance, freshness and
redaction posture.

```text
yai projection inspect --journal build/tmp/new8/projection-hardening-<pid>/journal.jsonl
```

## NEW.9 Query Boundary Loop

```text
create residue
reload journal
query all records
query receipt records
query memory candidate records
query by case
append query_result records
build projection over query residue
```

`tests/smoke/query-boundary/test_query_boundary.c` proves that query scans
persisted journal records and filters by record kind and case without adding a
database or graph traversal layer.

```text
yai query summary --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl
yai query records --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl --kind receipt --limit 10
```

## NEW.10 Rust Engine R1 Loop

```text
create Rust engine through C shim
append JSON record lines
count records
count receipt records
query memory candidates
build projection summary JSON
free engine
```

`tests/smoke/rust-engine-r1/test_rust_engine_r1.c` proves that C can call the
Rust engine only through an opaque handle and serialized record input.

```text
yai engine summary --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl
```

## NEW.11 Daemon IPC Loop

```text
start yaid on isolated Unix socket
wait for socket
yai daemon status
yai daemon info
yai daemon shutdown
assert daemon exits
```

`tests/smoke/daemon-ipc/test_daemon_ipc.sh` proves that `yaid` is a local
resident endpoint and that `yai` can talk to it without exposing core
operation execution yet.

## NEW.12 Daemon-Backed Core Loop

```text
start yaid on isolated Unix socket
yai daemon run-minimum-loop
capture daemon-created journal
yai daemon journal-summary
yai daemon projection-summary
yai daemon run-filesystem-loop
verify sandboxed filesystem loop response
shutdown daemon
```

`tests/smoke/daemon-core-loop/test_daemon_core_loop.sh` proves that the first
case/subject/op/control/receipt/store/projection loop can cross the local
daemon boundary. It still avoids public API, auth, HTTP, process carrier,
model carrier and background runtime execution.

## SPINE.22 Filesystem & Runtime Layout Freeze

```text
rm -rf /tmp/yai-core-install-test /tmp/yai-core-home-test
make install-local PREFIX=/tmp/yai-core-install-test YAI_HOME=/tmp/yai-core-home-test
PATH=/tmp/yai-core-install-test/bin:$PATH yai --version
PATH=/tmp/yai-core-install-test/bin:$PATH yai info
YAI_HOME=/tmp/yai-core-home-test PATH=/tmp/yai-core-install-test/bin:$PATH yai doctor
PATH=/tmp/yai-core-install-test/bin:$PATH yaid --version
test -d /tmp/yai-core-home-test/run
test -d /tmp/yai-core-home-test/store
test -d /tmp/yai-core-home-test/log
test -d /tmp/yai-core-home-test/tmp
test -d /tmp/yai-core-home-test/cases
test -d /tmp/yai-core-home-test/sockets
test -d /tmp/yai-core-home-test/config
PATH=/tmp/yai-core-install-test/bin:$PATH yai daemon status --socket /tmp/yai-core-home-test/run/yaid.sock
PATH=/tmp/yai-core-install-test/bin:$PATH yai daemon run-minimum-loop --socket /tmp/yai-core-home-test/run/yaid.sock
PATH=/tmp/yai-core-install-test/bin:$PATH yai daemon shutdown --socket /tmp/yai-core-home-test/run/yaid.sock
make uninstall-local PREFIX=/tmp/yai-core-install-test
test ! -e /tmp/yai-core-install-test/bin/yai
test ! -e /tmp/yai-core-install-test/bin/yaid
```

SPINE.22 freezes local runtime layout validation. Uninstall removes installed
binaries only; it must not delete `YAI_HOME`, which is user data.

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
