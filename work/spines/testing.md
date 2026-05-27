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
file header standard
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
| `record store` | LMDB lookup status, writes and reads are visible without faking readiness or journal-backed records |
| `file header standard` | ownership/header doctrine exists before broad source header application |
| `carrier skeleton` | non-process carrier families are inspectable with no execution |

## REPO.HYGIENE.0 Header Standard Loop

```text
file-header-standard.md exists
agent-operating-appendix.md exists
wave-template requires File/header impact
operational-extraction-contract references the agent appendix
guard is integrated in check-docs
```

```text
make check-file-header-standard
make check-docs
```

## REPO.HYGIENE.1 Ownership Surface Loop

```text
Makefile has ownership/status comments
tools/checks/*.sh have YAI purpose headers
transitional shim READMEs retain Transitional C shim banners
engine/README.md names Rust operational data engine ownership
system/README.md names C host/system implementation
tests/smoke/README.md names smoke-test ownership
```

```text
make check-file-header-standard
make check-docs
```

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
filesystem-loop lab pack fixture files are staged before provider attach
daemon filesystem loop materializes subject:policy-pack and policy/projection/authority residue
yai doctor reports runtime and hot-state status
yai hot status handles missing, corrupt and valid snapshots
daemon status/info/minimum-loop commands are reachable
command surface inventory maps primitive -> view -> command -> lab test -> docs
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
mkdir -p "$YAI_RUN/pack-fixture/policies"
cp labs/filesystem-loop/pack-fixture/pack.yaml "$YAI_RUN/pack-fixture"/
cp labs/filesystem-loop/pack-fixture/policies/*.json "$YAI_RUN/pack-fixture/policies"/
python -m json.tool "$YAI_RUN/pack-fixture/policies/filesystem-sandbox-policy.json"
yai daemon run-filesystem-loop --socket "$YAI_SOCKET"
grep 'subject:policy-pack' "$JOURNAL"
grep 'policy:manual-filesystem-sandbox-v0' "$JOURNAL"
```

This proves the current filesystem-loop lab fixture posture. It does not imply
a generic pack runtime command exists.

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

`tests/smoke/hot-state-cli/test_hot_state_cli.sh` proves the stable lab/operator
surface for hot state and projection freshness.

The operator-facing lab runbook for this command surface is:

```text
labs/filesystem-loop/runbook.md
labs/filesystem-loop/runbook.md
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

## SPINE.30 LMDB Record Write Path Loop

```text
LMDB env opens under YAI_HOME/store/lmdb
daemon loop writes journal first
CLI mirrors daemon journal records into LMDB
records_by_id count is non-zero
records_by_case index count is non-zero
records_by_kind index count is non-zero
store status reports ready only after schema metadata exists
journal path still exists after LMDB mirror
```

`tests/smoke/record-store-write/test_record_store_write.sh` proves the write
path and aggregate summary surface.

```text
make smoke-spine30
target/debug/yai store status
target/debug/yai store summary
```

Manual installed-loop check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store status

mkdir -p /tmp/yai-home-test/run
/tmp/yai-install-test/bin/yaid --socket /tmp/yai-home-test/run/yaid.sock --foreground &
DAEMON_PID=$!
sleep 1

PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store status
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store summary

PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock
wait $DAEMON_PID
```

Expected key lines after the daemon loop:

```text
record_store_backend: lmdb
record_store_status: ready
records_total: 8
records_by_case: 8
records_by_kind: 8
```

In sandboxed command runners, daemon IPC smoke tests may need to run outside
the default sandbox because they connect to local Unix sockets. That is an
execution-environment caveat, not a change to the architecture. File-backed
daemon smoke targets that share `build/tmp` runtime homes should be run
serially; parallel `make smoke` and `make check` runs can collide on temporary
socket paths.

## SPINE.31 LMDB Record Read / Query Path Loop

```text
write records to LMDB through the daemon loop import path
store summary shows non-zero records
store record list --kind reads records_by_kind and resolves records_by_id
store record list --case reads records_by_case and resolves records_by_id
store record get reads records_by_id
missing record returns record: not_found
missing LMDB reports record_store_status instead of journal-only fake reads
```

`tests/smoke/record-store-read-query/test_record_store_read_query.sh` proves
the read/query surface.

```text
make smoke-spine31
target/debug/yai store record list --kind receipt --limit 10
target/debug/yai store record list --case case:new12-daemon --limit 10
target/debug/yai store record get rec:new12-min-receipt
target/debug/yai store record get rec:missing
```

Manual installed-loop check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test

mkdir -p /tmp/yai-home-test/run
/tmp/yai-install-test/bin/yaid --socket /tmp/yai-home-test/run/yaid.sock --foreground &
DAEMON_PID=$!
sleep 1

PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock

PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store summary
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record list --kind receipt --limit 10
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record list --case case:new12-daemon --limit 10
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record get rec:new12-min-receipt
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record get rec:missing

PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock
wait $DAEMON_PID
```

Expected key lines:

```text
record_store_status: ready
records_total: 8
record_kind: receipt
case_ref: case:new12-daemon
schema: yai.record.v1
payload:
record: not_found
```

## SPINE.32 LMDB Subject / Receipt Index Loop

```text
filesystem loop writes records to LMDB
store summary reports records_by_subject and records_by_receipt
store record list --subject reads records_by_subject and resolves records_by_id
store record list --receipt reads records_by_receipt and resolves records_by_id
missing subject returns records_total: 0
missing LMDB reports record_store_status instead of journal-only fake reads
```

`tests/smoke/record-store-subject-receipt-indexes/test_record_store_subject_receipt_indexes.sh`
proves the subject/receipt index surface.

```text
make smoke-spine32
target/debug/yai store record list --subject subject:filesystem-sandbox --limit 20
target/debug/yai store record list --receipt receipt:new12-fs-write --limit 10
target/debug/yai store record list --subject subject:missing --limit 10
```

Expected key lines:

```text
records_by_subject: <non-zero>
records_by_receipt: <non-zero>
filter: subject
filter_value: subject:filesystem-sandbox
filter: receipt
filter_value: receipt:new12-fs-write
records_total: 0
records: none
```

## SPINE.33 LMDB CLI + Manual Validation Loop

```text
store status before write reports not_initialized when install-local created the directory
store status after daemon-loop import reports ready
store summary shape includes all aggregate index counts
record list shape is filter/filter_value/records_total/limit/records
record get shape includes schema, id, kind, case, source and payload
missing record returns record: not_found
zero-result list returns records_total: 0 and records: none
no store command falls back to journal for missing data
```

`tests/smoke/record-store-cli-manual-validation/test_record_store_cli_manual_validation.sh`
freezes the SPINE.33 command surface.

```text
make smoke-spine33
target/debug/yai store status
target/debug/yai store summary
target/debug/yai store record list --case case:new12-daemon --limit 10
target/debug/yai store record list --kind receipt --limit 10
target/debug/yai store record list --subject subject:filesystem-sandbox --limit 10
target/debug/yai store record list --receipt receipt:new12-fs-write --limit 10
target/debug/yai store record get rec:new12-min-receipt
target/debug/yai store record get rec:missing
```

Manual installed matrix:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store status
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai doctor

mkdir -p /tmp/yai-home-test/run
/tmp/yai-install-test/bin/yaid --socket /tmp/yai-home-test/run/yaid.sock --foreground &
DAEMON_PID=$!
sleep 1

PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store status
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store summary
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record list --case case:new12-daemon --limit 10
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record list --kind receipt --limit 10

PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai daemon run-filesystem-loop --socket /tmp/yai-home-test/run/yaid.sock
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store summary
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record list --subject subject:filesystem-sandbox --limit 10
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record list --receipt receipt:new12-fs-write --limit 10

REC_ID="$(PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store record list --kind receipt --limit 1 | awk '/record_id:/ {print $3; exit}')"
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store record get "$REC_ID"
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test yai store record get rec:missing
PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai store record list --subject subject:missing --limit 10

PATH=/tmp/yai-install-test/bin:$PATH YAI_HOME=/tmp/yai-home-test \
  yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock
wait $DAEMON_PID
make uninstall-local PREFIX=/tmp/yai-install-test
```

Expected key lines:

```text
record_store_status: not_initialized
record_store_status: ready
records_total: 8
records_by_case: 8
records_by_kind: 8
schema: yai.record.v1
filter: kind
filter_value: receipt
record_kind: receipt
case_ref: case:new12-daemon
source:
payload:
record: not_found
records_total: 0
records: none
```

## SPINE.33A Control / Carrier Substrate Loop

```text
carrier family enum/string roundtrip
gate outcome enum/string roundtrip
dispatch status enum/string roundtrip
receipt guarantee mode enum/string roundtrip
host observation posture enum/string roundtrip
dispatch struct initializes as pending/unknown
yai carrier families lists vocabulary and status without executing a carrier
```

`tests/smoke/control-carrier-substrate/test_control_carrier_substrate.c`
proves the C ABI primitives. The Makefile target also checks the CLI output.

```text
make check-control-carrier-substrate
make smoke-spine33a
target/debug/yai carrier families
```

Installed check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai carrier families
```

Expected key lines:

```text
carrier_families:
- filesystem
- process
- network_http
- database
- repository_git
- service
- endpoint
- socket
- listener
- model_provider
- observation
- review
current_status:
  filesystem: implemented_minimal carrier.v1
  process: implemented_minimal
gate_outcomes:
- allow
- deny
- require_review
- quarantine
dispatch_statuses:
- pending
- not_supported
receipt_guarantee_modes:
- observed
- interposed
- carrier_owned
- embedded
- external_import
```

## SPINE.33B Operation Dispatch Multiplex Loop

```text
dispatch lane enum/string roundtrip
dispatch plan initializes as unknown/not_supported
filesystem routes to filesystem_lane
process routes to process_lane
network_http routes to network_http_lane
database routes to database_lane
repository_git routes to repository_git_lane
model_provider routes to model_provider_lane
observation routes to observation_lane
review routes to review_lane
unknown family is not_supported
execution_performed is false
```

`tests/smoke/operation-dispatch-multiplex/test_operation_dispatch_multiplex.c`
proves the C ABI route helpers. The Makefile target also checks the CLI output.

```text
make check-operation-dispatch-multiplex
make smoke-spine33b
target/debug/yai carrier lanes
target/debug/yai carrier route --family filesystem
target/debug/yai carrier route --family process
target/debug/yai carrier route --family unknown
```

Installed check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai carrier lanes
PATH=/tmp/yai-install-test/bin:$PATH yai carrier route --family filesystem
PATH=/tmp/yai-install-test/bin:$PATH yai carrier route --family process
PATH=/tmp/yai-install-test/bin:$PATH yai carrier route --family unknown
```

Expected key lines:

```text
filesystem_lane
process_lane
network_http_lane
dispatch_status: routable
dispatch_status: not_supported
execution_performed: false
receipt_requirement: required
```

The filesystem loop lab includes these commands as dispatch
planning, not execution. Process lane is planned, filesystem lane is
active_minimal and execution remains false in SPINE.33B.

## SPINE.33C Carrier Contract Filesystem Loop

```text
carrier_contract:carrier.v1
carrier_outcome roundtrip
carrier_receipt init
filesystem carrier declares carrier.v1
filesystem supports read observed
filesystem supports write decision_required
filesystem receipt mapping covers observed/blocked/executed
no process/network/database/git/model execution occurs
```

`tests/smoke/carrier-contract-filesystem/test_carrier_contract_filesystem.c`
proves the C ABI contract primitives and filesystem receipt mapping.

```text
make check-carrier-contract-v1
make smoke-spine33c
target/debug/yai carrier inspect filesystem
target/debug/yai carrier route --family filesystem
```

Installed check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai carrier inspect filesystem
PATH=/tmp/yai-install-test/bin:$PATH yai carrier route --family filesystem
```

Expected key lines:

```text
carrier: filesystem
contract: carrier.v1
status: active_minimal
receipt_required: yes
read: observed
write: decision_required
pre_state_observation: supported
post_state_observation: supported
evidence_capture: supported
receipt_validation: supported
guarantee_mode: interposed
```

## SPINE.33D Process Carrier Signal Control Loop

```text
process carrier declares carrier.v1
process observe reports running for test-owned child
TERM dry-run is routable with carrier_attempted false
TERM real signal executes only for test-owned child
KILL real signal executes only for test-owned child
unsafe external PID signal path is blocked
no arbitrary kill
```

`tests/smoke/process-carrier/test_process_carrier.c` spawns child processes and
uses the C process carrier directly for real TERM/KILL in test-owned scope.
The CLI remains conservative and blocks arbitrary non-dry-run signal attempts.

```text
make check-process-carrier-signal-control
make smoke-spine33d
target/debug/yai carrier inspect process
target/debug/yai carrier route --family process
target/debug/yai process observe --pid $$
target/debug/yai process signal --pid $$ --signal TERM --dry-run
```

Expected key lines:

```text
carrier: process
contract: carrier.v1
status: active_minimal
platform: posix
state: running
carrier_attempted: false
expected_receipt: process_signal_receipt
reason: unsafe_process_target
```

## SPINE.33E Host Observation Probe Loop

```text
host probe process observation reports running/not_found/permission_denied
observation says enforcement none
observation_is_enforcement is false
compare expected running can match
compare expected stopped against running process yields mismatch
mismatch yields divergence_candidate
no silent repair
```

`tests/smoke/host-observation-probe/test_host_observation_probe.c` spawns a
test-owned child process, observes it, compares expected/observed state, emits
an `expected_stopped_but_running` divergence candidate, terminates the child
safely and confirms stopped/not_found posture.

```text
make check-host-observation-probe
make smoke-spine33e
target/debug/yai observe process --pid 1
target/debug/yai observe compare-process --pid 1 --expected running
target/debug/yai observe compare-process --pid 1 --expected stopped
```

Expected key lines:

```text
observation_target: process
enforcement: none
observation_is_enforcement: false
result: matched
result: mismatch
divergence_candidate: expected_stopped_but_running
silent_repair: false
```

## SPINE.33F Carrier Coverage Matrix Loop

```text
carrier mode enum/string roundtrip
carrier coverage table includes all required families
controlled/observed/imported modes are visible
filesystem controlled path is active_minimal
process controlled and observed paths are active_minimal
network_http/database/repository_git/service/endpoint/socket/listener are skeletons
unknown is unsupported
skeleton carrier families do not execute
```

`tests/smoke/carrier-coverage-matrix/test_carrier_coverage_matrix.c` proves
the C ABI table. The CLI smoke checks representative family filters.

```text
make check-carrier-coverage-matrix
make smoke-spine33f
target/debug/yai carrier coverage
target/debug/yai carrier coverage --family filesystem
target/debug/yai carrier coverage --family process
target/debug/yai carrier coverage --family database
target/debug/yai carrier coverage --family unknown
```

Installed check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai carrier coverage
PATH=/tmp/yai-install-test/bin:$PATH yai carrier coverage --family filesystem
PATH=/tmp/yai-install-test/bin:$PATH yai carrier coverage --family process
PATH=/tmp/yai-install-test/bin:$PATH yai carrier coverage --family database
PATH=/tmp/yai-install-test/bin:$PATH yai carrier coverage --family unknown
```

Expected key lines:

```text
carrier_coverage:
family: filesystem
controlled: active_minimal
family: process
observed: active_minimal
family: database
controlled: skeleton
execution_available: false
family: unknown
controlled: unsupported
```

## SPINE.33G Non-Process Carrier Skeleton Loop

```text
network_http/database/repository_git/service/endpoint/socket/listener inspectable
model_provider and review inspectable with special mode posture
all skeletons report carrier.v1
all skeletons report execution_available:false
all skeletons report receipt_required:yes
coverage matrix agrees with skeleton contract status
no real effects are executed
```

`tests/smoke/non-process-carrier-skeletons/test_non_process_carrier_skeletons.c`
proves the C ABI skeleton registry. The CLI smoke checks every skeleton family.

```text
make check-non-process-carrier-skeletons
make smoke-spine33g
target/debug/yai carrier inspect database
target/debug/yai carrier inspect network_http
target/debug/yai carrier inspect repository_git
target/debug/yai carrier inspect model_provider
target/debug/yai carrier coverage --family database
```

Installed check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai carrier inspect database
PATH=/tmp/yai-install-test/bin:$PATH yai carrier inspect endpoint
PATH=/tmp/yai-install-test/bin:$PATH yai carrier inspect model_provider
PATH=/tmp/yai-install-test/bin:$PATH yai carrier coverage --family database
```

Expected key lines:

```text
contract: carrier.v1
status: skeleton
execution_available: false
receipt_required: yes
non_execution_reason:
carrier_attempted: false
```

## SPINE.33H Carrier Outcome Harness Loop

```text
filesystem blocked posture
process blocked posture
database blocked posture
network_http failed posture
repository_git mismatch posture
service quarantined posture
endpoint observed posture
socket deferred posture
listener not_attempted posture
model_provider waiting_operator posture
review waiting_agent posture
unknown unsupported/not_attempted posture
skeleton carriers do not execute
mismatch generates divergence_candidate
invalid outcome fails cleanly
```

`tests/smoke/carrier-outcome-harness/test_carrier_outcome_harness.c` proves
the C ABI result posture. The CLI smoke checks representative active,
skeleton, mismatch, waiting and unsupported cases.

```text
make check-carrier-outcome-harness
make smoke-spine33h
target/debug/yai carrier outcome-test --family database --outcome blocked
target/debug/yai carrier outcome-test --family network_http --outcome failed
target/debug/yai carrier outcome-test --family repository_git --mode observed --outcome mismatch
target/debug/yai carrier outcome-test --family service --outcome quarantined
target/debug/yai carrier outcome-test --family model_provider --outcome waiting_operator
target/debug/yai carrier outcome-test --family review --outcome waiting_agent
target/debug/yai carrier outcome-test --family unknown --outcome blocked
```

Installed check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai carrier outcome-test --family database --outcome blocked
PATH=/tmp/yai-install-test/bin:$PATH yai carrier outcome-test --family network_http --outcome failed
PATH=/tmp/yai-install-test/bin:$PATH yai carrier outcome-test --family repository_git --mode observed --outcome mismatch
PATH=/tmp/yai-install-test/bin:$PATH yai carrier outcome-test --family service --outcome quarantined
PATH=/tmp/yai-install-test/bin:$PATH yai carrier outcome-test --family model_provider --outcome waiting_operator
PATH=/tmp/yai-install-test/bin:$PATH yai carrier outcome-test --family review --outcome waiting_agent
PATH=/tmp/yai-install-test/bin:$PATH yai carrier outcome-test --family unknown --outcome blocked
```

## SPINE.33I Carrier Receipt / Divergence Hardening Loop

```text
clean_executed consistent
clean_blocked consistent
denied_but_attempted critical divergence
executed_without_receipt error divergence
blocked_but_effect_observed critical divergence
receipt_claimed_executed_but_not_observed error divergence
failed_with_partial_effect error divergence
skeleton_executed_unexpectedly critical divergence
no carrier execution occurs
```

`tests/smoke/carrier-receipt-divergence/test_carrier_receipt_divergence.c`
proves the C ABI consistency primitive. The CLI smoke checks representative
clean and divergent scenarios.

```text
make check-carrier-receipt-divergence
make smoke-spine33i
target/debug/yai carrier reconcile-outcome --scenario clean_blocked
target/debug/yai carrier reconcile-outcome --scenario denied_but_attempted
target/debug/yai carrier reconcile-outcome --scenario executed_without_receipt
target/debug/yai carrier reconcile-outcome --scenario blocked_but_effect_observed
target/debug/yai carrier reconcile-outcome --scenario skeleton_executed_unexpectedly
```

Installed check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai carrier reconcile-outcome --scenario clean_blocked
PATH=/tmp/yai-install-test/bin:$PATH yai carrier reconcile-outcome --scenario denied_but_attempted
PATH=/tmp/yai-install-test/bin:$PATH yai carrier reconcile-outcome --scenario executed_without_receipt
PATH=/tmp/yai-install-test/bin:$PATH yai carrier reconcile-outcome --scenario blocked_but_effect_observed
PATH=/tmp/yai-install-test/bin:$PATH yai carrier reconcile-outcome --scenario skeleton_executed_unexpectedly
```

## SPINE.34 LMDB Record Plane Freeze Loop

```text
record store missing reports status
record store ready reports yai.record.v1
records_by_id/case/kind/subject/receipt are visible
decision records are queryable by kind
filesystem receipt records are queryable by receipt
subject-scoped records are queryable by subject
record get returns schema/source/payload
missing records return record: not_found only when LMDB is ready
no journal fallback after LMDB environment is removed
carrier_request/effect_receipt/divergence records are store/index capable
```

`tests/smoke/record-store-freeze/test_record_store_freeze.sh` proves the CLI
freeze. The Rust LMDB store unit test proves carrier/control/divergence record
kinds can be persisted and indexed.

```text
make check-lmdb-record-plane-freeze
make smoke-spine34
```

## SPINE.35 Journal Replay Parser Hardening Loop

```text
valid journal inspect works
missing journal handled cleanly
corrupt json line reported
empty journal handled cleanly
duplicate record ids reported
inspect does not write LMDB
inspect does not fake replay success
```

`tests/smoke/journal-replay-boundary/test_journal_replay_boundary.sh` proves
the parser hardening surface. It uses isolated temp journals and verifies
`lmdb_write: no`, `invalid_json`, `invalid_schema`, `duplicate` and honest
`replay_ready` output.

```text
make check-journal-replay-boundary
make smoke-spine35
```

## SPINE.36 Journal Replay to LMDB Loop

```text
journal replay dry-run writes nothing
valid journal replay writes records to LMDB
store summary reports ready and non-zero records
case-index list sees replayed records
second replay is idempotent
corrupt journal fails before write
```

`tests/smoke/journal-replay-to-lmdb/test_journal_replay_to_lmdb.sh` proves
journal replay to LMDB. It uses isolated temp journals and `YAI_HOME`, verifies
`records_written`, `records_duplicate`, `invalid_entries`, store summary and
case-index lookup.

```text
make check-journal-replay-to-lmdb
make smoke-spine36
```

## SPINE.37 Replay Idempotency + Schema Version Loop

```text
first replay writes records
second replay is idempotent
replay-status reports cursor
schema mismatch is rejected
missing schema reports invalid_schema
invalid input does not get false completed status
```

`tests/smoke/replay-idempotency-schema-version/test_replay_idempotency_schema_version.sh`
proves replay cursor metadata and schema compatibility. It uses isolated temp
journals and `YAI_HOME`, verifies `journal_identity`, `record_schema`,
`compatibility`, `cursor_line`, `replay_status`, `records_duplicate` and
`records_written: 0` for incompatible input.

```text
make check-replay-idempotency-schema-version
make smoke-spine37
```

## SPINE.38 Replay Diagnostics / Rebuild Report Loop

`tests/smoke/replay-diagnostics-report/test_replay_diagnostics_report.sh`
proves durable replay reports. It uses isolated temp journals and `YAI_HOME`,
then verifies real replay writes a report, `replay-report` reads the latest
report, second replay updates idempotent behavior, corrupt replay writes a
failed report, and the report schema is `yai.replay_report.v1`.

Required evidence includes `journal_identity`, `compatibility`, `cursor_line`,
`records_written`, `records_duplicate`, `invalid_entries` and failed report
diagnostics.

```text
make check-replay-diagnostics-report
make smoke-spine38
```

## SPINE.39 Journal Replay Freeze Loop

SPINE.39 adds no new replay semantics. It freezes the journal replay block as
an inspectable, diagnostic, idempotent, schema-aware and report-producing path
from append-only journal residue into LMDB.

`tests/smoke/journal-replay-freeze/test_journal_replay_freeze.sh` proves the
full compact lifecycle:

```text
journal inspect reports replay readiness
journal replay --dry-run writes nothing
journal replay materializes valid records into LMDB
journal replay-status reports journal_identity, compatibility and cursor_line
journal replay-report emits yai.replay_report.v1
second replay is idempotent
schema_mismatch blocks writes
invalid_json blocks writes and produces a failed report
store record list does not fall back to journal-only input
```

Required freeze fields include `journal_identity`, `record_schema`,
`journal_schema`, `compatibility`, `cursor_line`, `records_written`,
`records_duplicate`, `invalid_entries`, `replay_status` and
`replay_report_schema: yai.replay_report.v1`.

```text
make check-journal-replay-freeze
make smoke-spine39
```

## SPINE.40 Graph Persistence / RuntimeGraph Schema Loop

`tests/smoke/graph-runtimegraph-schema/test_graph_runtimegraph_schema.c`
proves graph node kind roundtrip, graph edge kind roundtrip and RuntimeGraph
boundary constants.

The CLI smoke verifies:

```text
yai graph schema prints node_kinds and edge_kinds
yai graph runtime-status says planned
runtime_graph role is in_memory_active_case_working_set
relation_write_path is active_minimal
```

The doctrine being protected is:

```text
Graph persistence owns durable typed relations.
RuntimeGraph is the in-memory active case working set.
HNSW finds candidate nodes.
HNSW is not graph truth.
Context Compiler renders controlled views.
Projection does not disappear.
Persistent truth on disk.
Computational shape in memory.
```

```text
make check-graph-runtimegraph-doctrine
make smoke-spine40
```

## SPINE.41 Graph Relation Write Path Loop

`tests/smoke/graph-relation-write-path/test_graph_relation_write_path.sh`
replays a small journal into LMDB, materializes graph relations and lists them
by case.

The CLI smoke verifies:

```text
yai graph materialize --case case:spine41
schema: yai.graph_relation.v1
relations_written non-zero
second materialize is duplicate-aware
yai graph relations --case case:spine41 --limit 30
edge_kind: decision_controls_attempt
edge_kind: receipt_records_effect
runtime_graph_updated: false
```

SPINE.41 keeps RuntimeGraph remains planned. Ladybug integration remains future
while proving typed graph relation write path behavior.

```text
make check-graph-relation-write-path
make smoke-spine41
```

## SPINE.42 RuntimeGraph Working Set Loop

`tests/smoke/runtimegraph-working-set/test_runtimegraph_working_set.sh`
replays records to LMDB, materializes graph relations and loads an ephemeral
RuntimeGraph working set.

The CLI smoke verifies:

```text
yai graph runtime-status
status: active_minimal
working_set: per_command_ephemeral
resident_service: planned
yai graph runtime-load --case case:spine42
nodes_total non-zero
edges_total non-zero
outgoing_index_entries non-zero
incoming_index_entries non-zero
case:missing nodes_total: 0
case:missing edges_total: 0
hnsw: future_candidate_index
durable_truth: graph_persistence
context_compiler: future_consumer
```

RuntimeGraph is an in-memory working set. Graph persistence is durable truth.
RuntimeGraph is loaded from graph relations. HNSW remains future candidate
index. Context Compiler remains future consumer.
The SPINE.42 working set is per-command ephemeral.

```text
make check-runtimegraph-working-set
make smoke-spine42
```

## SPINE.33L Provider Runtime / LAN Target Surface Loop

```text
device list empty
device add works
device inspect works
device trust works
provider runtime status works
provider targets works
provider start dry-run local works
provider start dry-run lan untrusted blocks
provider start dry-run lan trusted plans
provider start dry-run external attach_only works
provider logs-path works
model catalog status works
model runtime status works
no provider process is started
```

`tests/smoke/provider-runtime-surface/test_provider_runtime_surface.sh` proves
the surface v0 CLI behavior. It uses an isolated `YAI_HOME`, writes only the
device registry and provider runtime path directories, and verifies
`execution_performed: false`.

```text
make check-provider-runtime-lan-target-surface
make smoke-spine33l
target/debug/yai device list
target/debug/yai device add --id workstation --name Workstation --host 192.168.1.50 --port 8777 --target lan
target/debug/yai device inspect workstation
target/debug/yai device trust workstation
target/debug/yai provider runtime status
target/debug/yai provider targets
target/debug/yai provider start --dry-run --target lan --device workstation --kind ds4 --model deepseek-v4-flash
target/debug/yai provider logs-path
target/debug/yai model catalog status
target/debug/yai model runtime status
```

Installed check:

```text
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai device list
PATH=/tmp/yai-install-test/bin:$PATH yai device add --id workstation --name Workstation --host 192.168.1.50 --port 8777 --target lan
PATH=/tmp/yai-install-test/bin:$PATH yai device trust workstation
PATH=/tmp/yai-install-test/bin:$PATH yai provider start --dry-run --target lan --device workstation --kind ds4 --model deepseek-v4-flash
PATH=/tmp/yai-install-test/bin:$PATH yai model runtime status
```

Expected key lines:

```text
execution_performed: false
carrier_attempted: false
receipt_required: yes
receipt_posture: simulated
divergence_candidate:
carrier_status: unsupported
```

## SPINE.33M Data Context Runtime / RuntimeGraph Doctrine Loop

```text
Truth lives on durable planes.
Computation happens in runtime working sets.
LMDB stores records.
Ladybug stores relations.
DuckDB stores facts.
RuntimeGraph computes over the active case.
HNSW finds candidate nodes.
Context Compiler renders controlled views.
SPINE.58D HNSW Candidate -> RuntimeGraph Expansion
```

SPINE.33M is docs/roadmap/guard only. It does not implement RuntimeGraph,
HNSW, graph persistence, context compilation, retrieval commands or model
runtime behavior.

```text
make check-data-context-runtime-roadmap
make check-docs
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

## SPINE.43 RuntimeGraph Rebuild Loop

`tests/smoke/runtimegraph-rebuild/test_runtimegraph_rebuild.sh` validates the
RuntimeGraph rebuild chain:

```text
journal -> LMDB -> graph relations -> runtime_graph_rebuild -> runtime-summary
```

The smoke verifies journal rebuild, report readback, graph-relations rebuild,
duplicate-aware second rebuild, missing-case zero behavior, bad-journal failed
status and resident service planned posture.

```bash
make check-runtimegraph-rebuild
make smoke-spine43
```

Required command surface:

```bash
yai graph rebuild --case <case_ref> --from journal --path <journal.jsonl>
yai graph rebuild-report --case <case_ref>
yai graph runtime-summary --case <case_ref>
yai graph rebuild --case <case_ref> --from graph-relations
```

Required output includes `yai.runtime_graph_rebuild_report.v1`,
`runtime_graph_rebuild`, `journal`, `LMDB`, `graph relations`,
`runtime-summary`, `resident service planned` and the rule that RuntimeGraph is
not durable truth. RuntimeGraph is not durable truth.

## SPINE.44 RuntimeGraph Query / Causal Path Loop

`tests/smoke/runtimegraph-query/test_runtimegraph_query.sh` validates
RuntimeGraph fanout, fanin, neighborhood and causal path diagnostics.

```bash
yai graph fanout --case <case_ref> --node <ref>
yai graph fanin --case <case_ref> --node <ref>
yai graph neighborhood --case <case_ref> --node <ref> --depth 1
yai graph path --case <case_ref> --from <ref> --to <ref> --max-depth 4
```

RuntimeGraph is not a generic graph database. The smoke verifies fanout,
fanin, neighborhood, causal path found, causal path not_found, edge-kind
filter behavior, bounded traversal, max-depth clamping and empty-case output.
Plain output remains parseable. Color-aware graph inspection is documented for
future console/studio use under `labs/filesystem-loop`.

Guard vocabulary: RuntimeGraph is not a generic graph database; fanout, fanin,
neighborhood, bounded traversal, causal path, max-depth, edge-kind filter,
color-aware graph inspection, plain output remains parseable.

## SPINE.44A Operator Review / Deferred Action Loop

`tests/smoke/operator-review-loop/test_operator_review_loop.sh` validates the
filesystem review loop:

```bash
yai daemon run-filesystem-review-loop --socket <socket>
yai control pending --case case:new12-filesystem
yai control show review:new12-fs-write-review
yai control approve review:new12-fs-write-review --reason "smoke approve"
```

The smoke covers outside-sandbox denial, `require_review` to
`pending_operator`, approve, deny, defer, quarantine, persisted review records,
filesystem receipts and graph materialization over review/control records.
The review loop is scriptable: it emits `review_required: yes`, `status:
pending_operator`, `carrier_attempted: false`, `execution_performed: false`
and `next_commands`, then exits without waiting for operator input.
`subject:linenoise-terminal is prompt surface`; operator reviewer authority is
separate. Deny, defer and quarantine keep `carrier_attempted: false` and
`execution_performed: false`.

Guard vocabulary: require_review, pending_operator, approve, deny, defer,
quarantine, carrier_attempted: false, execution_performed: false,
subject:linenoise-terminal is prompt surface, operator reviewer authority,
labs/filesystem-loop.

## SPINE.44B CLI Review Interaction Surface

`tests/smoke/operator-review-cli/test_operator_review_cli.sh` validates the
scriptable and interactive-facing review CLI:

```bash
yai control pending --case case:new12-filesystem
yai control show review:new12-fs-write-review
yai control review --case case:new12-filesystem --interactive
yai control watch --case case:new12-filesystem --interval-ms 500 --max-events 5
yai control wait review:new12-fs-write-review --timeout 1
```

The smoke checks that `control pending` prints `next_commands`, `control show`
exposes the authority boundary, non-TTY interactive review returns
`not_a_tty`, watch reports a pending event and wait reports timeout before
resolution. `approve` executes the reviewed filesystem write; `deny`, `defer`
and `quarantine` keep `carrier_attempted: false` and
`execution_performed: false`.

Authority boundary:

```text
subject:linenoise-terminal is prompt surface
subject:operator-reviewer is review authority
```

Guard vocabulary: control pending, control show, control review --interactive,
control watch, control wait, next_commands, not_a_tty, pending_operator,
approve, deny, defer, quarantine, carrier_attempted: false,
execution_performed: false, subject:linenoise-terminal is prompt surface,
subject:operator-reviewer is review authority, labs/filesystem-loop.

## SPINE.44C Review Loop Test Matrix

`tests/smoke/review-loop-test-matrix/test_review_loop_test_matrix.sh`
validates the complete review loop matrix:

```bash
make smoke-spine44c
```

The smoke verifies blocked outside-sandbox writes, `pending_operator`,
`next_commands`, approve, deny, defer, quarantine, `wait timeout`, resolved
wait, bounded `watch` and non-TTY interactive handling. Non-execution paths
keep `carrier_attempted: false` and `execution_performed: false`.

Expected smoke labels:

```text
review_matrix:blocked ok
review_matrix:pending ok
review_matrix:approve executed ok
review_matrix:deny no_execution ok
review_matrix:defer no_execution ok
review_matrix:quarantine no_execution ok
review_matrix:next_commands ok
review_matrix:wait_timeout ok
review_matrix:wait_resolved ok
review_matrix:watch ok
review_matrix:non_tty ok
```

Lab alignment lives in `labs/filesystem-loop` and
`labs/filesystem-loop`. The model lab records model proposal observed,
model cannot approve, and automatic proposed-op gate import is future work.

## SPINE.45 Graph + RuntimeGraph Freeze

`tests/smoke/graph-runtimegraph-freeze/test_graph_runtimegraph_freeze.sh`
validates the frozen graph lifecycle: graph schema, `yai.graph_relation.v1`,
graph materialize, graph relations, RuntimeGraph runtime-load/runtime-summary,
runtime graph rebuild, `yai.runtime_graph_rebuild_report.v1`, fanout, fanin,
neighborhood, causal path, bounded traversal, edge-kind filter, path found,
path not_found and empty case.

Expected labels:

```text
graph_freeze:schema ok
graph_freeze:materialize ok
graph_freeze:relations ok
graph_freeze:runtime_load ok
graph_freeze:runtime_rebuild ok
graph_freeze:rebuild_report ok
graph_freeze:fanout ok
graph_freeze:fanin ok
graph_freeze:neighborhood ok
graph_freeze:path_found ok
graph_freeze:path_not_found ok
graph_freeze:empty_case ok
graph_freeze:bounded ok
graph_freeze:review_records visible_or_classified
```

RuntimeGraph is not durable truth; it remains per-command ephemeral. Plain
output remains parseable and color-aware graph inspection remains doctrine.
HNSW future, Context Compiler future and Ladybug future persistence integration
are non-goals.

Review/control coverage checks `review_request`, `review_decision`,
`control_pending`, approve, deny, defer and quarantine. The approve path is
graph/query visible; deny, defer and quarantine no-execution posture remains
covered by SPINE.44C.

## SPINE.45A Documentation Root Canon Collapse

`tools/checks/check-doc-root-canon.sh` validates the documentation root canon:

```bash
make check-doc-root-canon
make check-docs
```

The guard fails if active `docs/internal`, `docs/engineering`, `docs/spines`,
`docs/product`, `docs/reference`, `docs/status`, `docs/protocols`, `docs/adr`,
`docs/labs` or `docs/lab-standards` roots return. It also verifies the compact
architecture docs, `labs/`, `labs/standards`, `work/`, `work/spines` and
`work/inventories/extraction-inventory.tsv`.

## SPINE.45B Case Runtime Semantics Roadmap Rebase

`tools/checks/check-case-runtime-semantics-roadmap.sh` validates the SPINE.45B
roadmap/doctrine rebase:

```bash
make check-case-runtime-semantics-roadmap
make check-docs
```

The guard checks bitemporal fact fields, ContextDelta, CaseModelSession, KV
cache is not YAI memory, Native vs Attached Retrieval, Retrieval Provider
Contract, Retrieval Receipt, Federated Candidate Normalization, External
Candidate -> Case Import, Temporal Graph Revision, Graph Patch Operation,
Dependency Closure, Counterfactual, SPINE.58H-R and SPINE.65A-G.

## SPINE.46 DuckDB Fact Plane

`tests/smoke/duckdb-fact-plane/test_duckdb_fact_plane.sh` validates the
initial DuckDB fact-plane surface:

```bash
make check-duckdb-fact-plane
make smoke-spine46
```

Expected labels:

```text
fact_plane:status ok
fact_plane:schema ok
fact_plane:bitemporal ok
fact_plane:not_truth ok
fact_plane:init ok
fact_plane:tables ok
fact_plane:no_extraction confirmed
```

The smoke verifies `yai facts status`, `yai facts schema`, `yai facts init`,
schema `yai.fact.v1`, `transaction_time`, `valid_time_start`,
`valid_time_end`, `known_at`, `revision_of`, `superseded_by`,
`retracted_by`, `facts_are_truth: false`, twelve DuckDB tables and
`facts_extracted: 0`.

Facts are not truth. SPINE.46 has No fact extraction; extraction begins in
SPINE.47.

## SPINE.47 Receipt / Decision / Projection Facts

`tests/smoke/receipt-decision-projection-facts/test_receipt_decision_projection_facts.sh`
validates the first extraction path:

```bash
make check-receipt-decision-projection-facts
make smoke-spine47
```

Expected labels:

```text
facts_extract:receipt ok
facts_extract:decision ok
facts_extract:projection ok
facts_extract:core ok
facts_extract:idempotent ok
facts_summary:counts ok
facts:not_truth ok
```

The smoke runs the filesystem loop, replays the journal to LMDB, initializes
DuckDB, extracts `fact_receipt`, `fact_decision` and `fact_projection`, checks
`facts summary`, then verifies duplicate-aware idempotent extraction. Facts are
not truth. Deterministic fact IDs use `fact:<kind>:<source_record_id>`.
`transaction_time`, `valid_time_start`, `valid_time_end`, `known_at` and
revision fields are part of the schema. valid_time_end sentinel: 0 means
open-ended. No fact revision is implemented in SPINE.47.

## SPINE.48 Model Behavior / Policy Outcome Facts

`tests/smoke/model-behavior-policy-facts/test_model_behavior_policy_facts.sh`
validates the behavior extraction path:

```bash
make check-model-behavior-policy-facts
make smoke-spine48
```

Expected labels:

```text
facts_extract:model_behavior ok
facts_extract:policy_outcome ok
facts_extract:behavior ok
facts_extract:idempotent ok
facts_summary:behavior_counts ok
facts:not_truth ok
```

The smoke runs the filesystem loop, replays the journal to LMDB, initializes
DuckDB, extracts `core`, extracts `fact_model_behavior` and
`fact_policy_outcome`, then verifies duplicate-aware `behavior` extraction.
`fact_policy_outcome` must be greater than zero on the filesystem loop.
`fact_model_behavior` is honest and may be zero if no model records exist.
Facts are not truth. model proposal is not execution. model cannot approve.
automatic proposed-op gate import is future work. The extraction is
bitemporal, duplicate-aware and uses no LLM-based classification.
