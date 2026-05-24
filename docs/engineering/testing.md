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
| `record store` | LMDB lookup status is visible without faking write-path readiness |

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

## SPINE.24A Command Surface Recovery Loop

```text
runtime/install path commands are inspectable
pack and foundation guards are callable directly
manual policy pack fixtures are staged before provider attach
daemon filesystem loop materializes subject:policy-pack and policy/projection/authority residue
yai doctor reports runtime and hot-state status
yai hot status handles missing, corrupt and valid snapshots
daemon status/info/minimum-loop commands are reachable
command surface inventory maps primitive -> view -> command -> manual test -> docs
```

`tests/smoke/command-surface/test_command_surface.sh` proves the retroactive
operator surface for SPINE.20-SPINE.24.

```text
make smoke-spine24a
make print-install-paths PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
make check-source-surface-clean
make check-pack-doctrine
make check-foundation-freeze
target/debug/yai --version
target/debug/yai info
target/debug/yai doctor
target/debug/yai hot status
build/yaid --version
```

Manual pack fixture checkpoint:

```text
cp docs/manuals/examples/filesystem-loop/policy-packs/*.json "$YAI_RUN/policy-packs"/
python -m json.tool "$YAI_RUN/policy-packs/filesystem-sandbox-policy.json"
yai daemon run-filesystem-loop --socket "$YAI_SOCKET"
grep 'subject:policy-pack' "$JOURNAL"
grep 'policy:manual-filesystem-sandbox-v0' "$JOURNAL"
```

This proves the current manual posture. It does not imply a `yai pack`
runtime command exists.

## SPINE.28B Source Surface Cleanup

```text
system/ is not a second data engine
engine/ is the Rust data spine target
venv/.venv/env/ENV are blocked from the repo
system/ingest and include/yai/ingest stay archived until ingest work
transitional C data shims carry a standard banner
```

Manual check:

```text
make check-source-surface-clean
find . -maxdepth 2 -type d -name "venv" -o -name ".venv"
```

Expected:

```text
check-source-surface-clean: ok
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

## SPINE.26 Projection Freshness Policy Loop

```text
fresh model projection is usable
receipt-stale model projection requires refresh
operator/debug stale projection remains inspectable with warning
authority/thread stale model projection is blocked_for_model
projection refresh returns usable
```

`tests/smoke/projection-freshness/test_projection_freshness.c` proves the
consumer-aware policy classification. The daemon-core-loop smoke also checks
that `yai projection inspect` and prompt dry-run expose freshness policy.

```text
make smoke-spine26
yai projection inspect --journal <journal> --consumer model
yai prompt --dry-run --once "..."
```

## SPINE.27 Hot State CLI Manual Validation Loop

```text
yai info names SPINE.27
yai doctor reports hot_state_path/status/schema/readability
yai hot status reports missing, corrupt and valid snapshot cases
daemon minimum loop produces active hot-state status
daemon filesystem loop produces active case/session/context status
projection inspect exposes projection_freshness/stale_reason/freshness_policy/freshness_source
```

`tests/smoke/hot-state-cli/test_hot_state_cli.sh` proves the stable manual
surface for hot state and projection freshness.

The operator-facing runbook and notebooks for this command surface are:

```text
docs/manuals/manual-filesystem-loop-validation.md
docs/manuals/manual-filesystem-loop-validation.ipynb
docs/manuals/manual-filesystem-loop-validation.it.ipynb
```

```text
make smoke-spine27
target/debug/yai --version
target/debug/yai info
target/debug/yai doctor
target/debug/yai hot status
target/debug/yai projection inspect --journal <journal> --consumer model
```

Expected key lines:

```text
hot_state: active|unavailable
hot_state_schema_status: valid|invalid|missing
schema: yai.hot_state.v1
case_session: active
case_world: loaded
case_context: active
projection: fresh|stale
freshness_policy: usable|refresh_recommended|refresh_required|blocked_for_model|unknown
freshness_source: hot_state|projection_record
```

## SPINE.29 LMDB Record Plane Doctrine Loop

```text
yai store status exists
record_store_path resolves under YAI_HOME/store/lmdb
record_store_backend is lmdb
record_store_status is missing or not_initialized, never fake ready
yai doctor includes record store fields
doctrine guard checks keyspace, schema and plane boundaries
```

`tests/smoke/record-store-cli/test_record_store_cli.sh` proves the SPINE.29
status surface without adding an LMDB dependency or opening an environment.

```text
make check-lmdb-record-plane-doctrine
make smoke-spine29
target/debug/yai store status
target/debug/yai doctor
```

Installed layout check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai store status
PATH=/tmp/yai-install-test/bin:$PATH yai doctor
```

Expected key lines:

```text
record_store_backend: lmdb
record_store_status: missing|not_initialized|unavailable
record_store_path: /tmp/yai-home-test/store/lmdb
schema: yai.record.v1
```

## SPINE.28 Hot State Freeze Loop

```text
hot state remains explicitly non-authoritative
snapshot schema remains yai.hot_state.v1
missing_snapshot and invalid_snapshot remain documented status cases
case_session/case_world/case_context fields remain diagnostic cache
projection_freshness/stale_reason/freshness_policy remain visible
hot-state smoke suite remains in make smoke/check
LMDB boundary states durable lookup will not replace hot state
```

SPINE.28 is a freeze, not a feature wave. It validates that SPINE.23 through
SPINE.27 remain stable before SPINE.29 starts the LMDB record plane.

```text
make check-hot-state-freeze
make smoke-spine23
make smoke-spine24
make smoke-spine24a
make smoke-spine25
make smoke-spine26
make smoke-spine27
```

Manual minimum matrix:

```text
target/debug/yai doctor
target/debug/yai hot status
yai daemon run-minimum-loop --socket <socket>
yai daemon run-filesystem-loop --socket <socket>
yai hot status
yai projection inspect --journal <journal>
rm -f $YAI_HOME/run/hot-state.json && yai hot status
printf '{broken\n' > $YAI_HOME/run/hot-state.json && yai hot status
```

Expected key lines:

```text
hot_state: active
schema: yai.hot_state.v1
case_session:
case_world:
case_context:
projection:
freshness_policy:
hot_state: unavailable
reason: missing_snapshot
hot_state: unavailable
reason: invalid_snapshot
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
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai --version
PATH=/tmp/yai-install-test/bin:$PATH yai info
YAI_HOME=/tmp/yai-home-test PATH=/tmp/yai-install-test/bin:$PATH yai doctor
PATH=/tmp/yai-install-test/bin:$PATH yaid --version
test -d /tmp/yai-home-test/run
test -d /tmp/yai-home-test/store
test -d /tmp/yai-home-test/log
test -d /tmp/yai-home-test/tmp
test -d /tmp/yai-home-test/cases
test -d /tmp/yai-home-test/sockets
test -d /tmp/yai-home-test/config
PATH=/tmp/yai-install-test/bin:$PATH yai daemon status --socket /tmp/yai-home-test/run/yaid.sock
PATH=/tmp/yai-install-test/bin:$PATH yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock
PATH=/tmp/yai-install-test/bin:$PATH yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock
make uninstall-local PREFIX=/tmp/yai-install-test
test ! -e /tmp/yai-install-test/bin/yai
test ! -e /tmp/yai-install-test/bin/yaid
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
