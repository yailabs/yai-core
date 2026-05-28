# Command Surface

This is the active operator command index for `yai`.

Rule:

```text
A primitive that cannot be inspected is not operational yet.
```

This file maps SPINE.20-SPINE.32 primitives to their current view, command,
lab test and documentation surface. It does not define new core semantics.
The operator-facing runbook is `labs/filesystem-loop/runbook.md`.
The canonical operator surface is the lab tree; the old manuals surface has been removed.

## Runtime Commands

| Primitive | View | Command | Lab test | Docs |
|---|---|---|---|---|
| `YAI_HOME` | resolved runtime home | `yai doctor` | `target/debug/yai doctor` | `filesystem-target.md`, `testing.md` |
| runtime dirs | resolved `run/store/log/tmp/cases/sockets/config` | `yai doctor` | `target/debug/yai doctor` | `filesystem-target.md` |
| install paths | Makefile install map | `make print-install-paths` | `make print-install-paths PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test` | `filesystem-target.md` |
| local install | binaries plus runtime dirs | `make install-local` | `make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test` | `testing.md` |
| local uninstall | remove installed binaries only | `make uninstall-local` | `make uninstall-local PREFIX=/tmp/yai-install-test` | `testing.md` |
| local doctor | installed binary/path diagnostic | `make doctor-local` | `make doctor-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test` | `testing.md` |

`yai doctor` is the canonical runtime path view. There is no separate
`yai runtime paths` command in SPINE.24A.

## Daemon Commands

| Primitive | View | Command | Lab test | Docs |
|---|---|---|---|---|
| `yaid` binary | daemon version | `yaid --version` | `/tmp/yai-install-test/bin/yaid --version` | `testing.md` |
| local socket | daemon foreground endpoint | `yaid --socket <path> --foreground` | `yaid --socket /tmp/yai-home-test/run/yaid.sock --foreground` | `filesystem-target.md` |
| daemon liveness | status response | `yai daemon status --socket <path>` | `yai daemon status --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| daemon metadata | daemon info response | `yai daemon info --socket <path>` | `yai daemon info --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| minimum loop | daemon-backed control loop | `yai daemon run-minimum-loop --socket <path>` | `yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| filesystem loop | daemon-backed filesystem loop | `yai daemon run-filesystem-loop --socket <path>` | `yai daemon run-filesystem-loop --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| shutdown | daemon stop request | `yai daemon shutdown --socket <path>` | `yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |

## Hot State Commands

| Primitive | View | Command | Lab test | Docs |
|---|---|---|---|---|
| hot state | live cache status | `yai hot status` | `target/debug/yai hot status` | `hot-state-plane.md`, `data-plane-roadmap.md` |
| hot snapshot path | snapshot source | `yai hot status` | `YAI_HOME=/tmp/yai-home-test yai hot status` | `filesystem-target.md` |
| snapshot schema | `yai.hot_state.v1` validation | `yai hot status` | valid daemon snapshot after `run-minimum-loop` | `hot-state-plane.md` |
| missing snapshot | unavailable reason | `yai hot status` | `rm -f $YAI_HOME/run/hot-state.json; yai hot status` | `testing.md` |
| invalid snapshot | unavailable reason | `yai hot status` | `printf '{broken\n' > $YAI_HOME/run/hot-state.json; yai hot status` | `testing.md` |
| doctor hot state | path/readability/schema | `yai doctor` | `target/debug/yai doctor` | `testing.md` |

Required `yai hot status` fields:

```text
hot_state: active|unavailable
snapshot: <path>
schema: yai.hot_state.v1|unknown
case_session: active|inactive|unknown
case_world: loaded|not_loaded|unknown
case_context: active|inactive|unknown
active_thread: <id>|none|unknown
participant_view: <id>|none|unknown
projection: fresh|stale|unknown|rebuilding
freshness_policy: usable|refresh_required|refresh_recommended|blocked_for_model|unknown
stale_reason: ...
updated_at: ...
```

`projection_policy` may also appear as a compatibility alias for the same
value. `freshness_policy` is the canonical field.

There is no `yai hot status --json` convention in SPINE.27.

SPINE.28 freezes this command surface before LMDB work. The stable hot-state
operator fields are `hot_state`, `snapshot`, `schema`, `case_session`,
`case_world`, `case_context`, `projection`, `stale_reason`,
`freshness_policy`, `updated_at`, plus latest decision/receipt refs when
available. LMDB will add durable record lookup later; it will not replace
`yai hot status`.

## Record Store Commands

| Primitive | View | Command | Lab test | Docs |
|---|---|---|---|---|
| record store | LMDB record-plane readiness | `yai store status` | `target/debug/yai store status` | `lmdb-record-plane.md`, `data-plane-roadmap.md` |
| record store summary | LMDB aggregate write-path counts | `yai store summary` | `target/debug/yai store summary` | `lmdb-record-plane.md`, `testing.md` |
| record get | LMDB id lookup | `yai store record get <record_id>` | `target/debug/yai store record get rec:new12-min-receipt` | `lmdb-record-plane.md`, `testing.md` |
| record list by case | LMDB case index scan | `yai store record list --case <case_ref> [--limit <N>]` | `target/debug/yai store record list --case case:new12-daemon --limit 10` | `lmdb-record-plane.md`, `testing.md` |
| record list by kind | LMDB kind index scan | `yai store record list --kind <record_kind> [--limit <N>]` | `target/debug/yai store record list --kind receipt --limit 10` | `lmdb-record-plane.md`, `testing.md` |
| record list by subject | LMDB subject index scan | `yai store record list --subject <subject_ref> [--limit <N>]` | `target/debug/yai store record list --subject subject:filesystem-sandbox --limit 20` | `lmdb-record-plane.md`, `testing.md` |
| record list by receipt | LMDB receipt index scan | `yai store record list --receipt <receipt_ref> [--limit <N>]` | `target/debug/yai store record list --receipt receipt:new12-fs-write --limit 10` | `lmdb-record-plane.md`, `testing.md` |
| journal inspect | replay-readiness diagnostics without LMDB writes | `yai journal inspect --path <journal.jsonl> [--show-errors]` | `target/debug/yai journal inspect --path /tmp/yai-journal.jsonl --show-errors` | `journal-replay-parser-hardening.md`, `testing.md` |
| journal replay | materialize valid journal records into LMDB | `yai journal replay --path <journal.jsonl> [--dry-run]` | `target/debug/yai journal replay --path /tmp/yai-journal.jsonl --dry-run` | `journal-replay-to-lmdb.md`, `testing.md` |
| journal replay status | replay cursor/schema metadata | `yai journal replay-status --path <journal.jsonl>` | `target/debug/yai journal replay-status --path /tmp/yai-journal.jsonl` | `replay-idempotency-schema-version.md`, `testing.md` |
| journal replay report | durable replay diagnostics | `yai journal replay-report --path <journal.jsonl>` | `target/debug/yai journal replay-report --path /tmp/yai-journal.jsonl` | `replay-diagnostics-rebuild-report.md`, `testing.md` |
| doctor record store | path/backend/status | `yai doctor` | `target/debug/yai doctor` | `lmdb-record-plane.md`, `testing.md` |
| carrier family vocabulary | control/carrier substrate posture | `yai carrier families` | `target/debug/yai carrier families` | `control-carrier-rebase.md`, `testing.md` |
| carrier lane vocabulary | no-execution lane metadata | `yai carrier lanes` | `target/debug/yai carrier lanes` | `operation-dispatch-multiplex.md`, `testing.md` |
| carrier route plan | no-execution route inspection | `yai carrier route --family <family>` | `target/debug/yai carrier route --family filesystem` | `operation-dispatch-multiplex.md`, `testing.md` |
| carrier contract inspection | filesystem carrier.v1 posture | `yai carrier inspect filesystem` | `target/debug/yai carrier inspect filesystem` | `carrier-contract-v1.md`, `testing.md` |
| process carrier inspection | process carrier.v1 posture | `yai carrier inspect process` | `target/debug/yai carrier inspect process` | `process-carrier-signal-control.md`, `testing.md` |
| process observe | process state observation | `yai process observe --pid <pid>` | `target/debug/yai process observe --pid $$` | `process-carrier-signal-control.md`, `testing.md` |
| process signal dry-run | no-effect signal planning | `yai process signal --pid <pid> --signal TERM --dry-run` | `target/debug/yai process signal --pid $$ --signal TERM --dry-run` | `process-carrier-signal-control.md`, `testing.md` |
| host process observation | independent host probe | `yai observe process --pid <pid>` | `target/debug/yai observe process --pid 1` | `host-observation-probe.md`, `testing.md` |
| host process comparison | expected/observed mismatch view | `yai observe compare-process --pid <pid> --expected running|stopped` | `target/debug/yai observe compare-process --pid 1 --expected stopped` | `host-observation-probe.md`, `testing.md` |
| carrier coverage matrix | family/mode/outcome coverage visibility | `yai carrier coverage [--family <family>] [--mode <mode>]` | `target/debug/yai carrier coverage --family process` | `carrier-coverage-matrix.md`, `testing.md` |
| non-process carrier skeletons | inspectable carrier.v1 no-execution skeletons | `yai carrier inspect <family>` | `target/debug/yai carrier inspect database` | `non-process-carrier-skeletons.md`, `testing.md` |
| carrier outcome harness | dry-run outcome posture over the carrier matrix | `yai carrier outcome-test --family <family> --outcome <outcome>` | `target/debug/yai carrier outcome-test --family database --outcome blocked` | `carrier-outcome-harness.md`, `testing.md` |
| carrier receipt divergence | consistency posture across decision/dispatch/outcome/receipt/observation | `yai carrier reconcile-outcome --scenario <scenario>` | `target/debug/yai carrier reconcile-outcome --scenario denied_but_attempted` | `carrier-receipt-divergence.md`, `testing.md` |

`yai store status` is the readiness view because `store` already names the
durable data root and LMDB is the record-plane backend under it. SPINE.30 adds
`yai store summary` for aggregate write-path validation. SPINE.31 adds
read-only `yai store record get` and `yai store record list` over the id, case
and kind indexes. SPINE.32 extends list with subject and receipt filters.
SPINE.33 freezes the line-oriented operator output shape before the record-plane
freeze. Future subcommands can extend the same surface with
`yai store rebuild status`.

SPINE.34 adds no new command. It freezes the existing LMDB record-plane command
surface, including id/case/kind/subject/receipt lookup, `yai.record.v1`,
`records_by_id`, `records_by_case`, `records_by_kind`, `records_by_subject`,
`records_by_receipt` and no journal fallback reads.

SPINE.35 adds `yai journal inspect --path <journal.jsonl> [--show-errors]`.
This is replay-readiness inspection only. It reports parser diagnostics and
`replay_ready: yes|no`; it does not write LMDB, rebuild LMDB or change no
journal fallback behavior for LMDB query commands.

SPINE.36 adds `yai journal replay --path <journal.jsonl> [--dry-run]`. Dry-run
reports replay readiness and intended writes without opening LMDB. Real replay
writes valid records through the existing LMDB writer and index path. Existing
records are counted as `records_duplicate`, so a second replay is idempotent.

SPINE.37 adds `yai journal replay-status --path <journal.jsonl>` and extends
replay output with replay cursor metadata: `journal_identity`, `record_schema`,
`journal_schema`, `compatibility`, `cursor_line` and `replay_status`.

SPINE.38 adds durable replay diagnostics. Real replay writes
`yai.replay_report.v1` under `YAI_HOME/store/replay/reports/` and prints
`replay_report: <path>`. `yai journal replay-report --path <journal.jsonl>`
prints `journal_identity`, `compatibility`, `cursor_line`, `records_written`,
`records_duplicate`, `invalid_entries`, idempotent status and failed report
diagnostics.

SPINE.39 adds no command. It freezes the journal replay command surface and
validates `journal inspect`, `journal replay --dry-run`, `journal replay`,
`journal replay-status`, `journal replay-report`, `yai.replay_report.v1`,
`schema_mismatch`, `invalid_json`, no silent skip, no false complete and no
journal fallback.

Required fields:

```text
record_store_backend: lmdb
record_store_status: missing|not_initialized|ready|unavailable
record_store_path: <YAI_HOME>/store/lmdb
record_env: record_env
schema: yai.record.v1
indexes: records_by_id,records_by_case,records_by_kind,records_by_subject,records_by_receipt
```

`missing` means `YAI_HOME/store/lmdb` is absent. `not_initialized` means the
directory exists but no LMDB schema metadata has been initialized. `ready` means
the LMDB environment opens and `schema_meta/meta:schema` is `yai.record.v1`.
The `indexes` line is emitted only for `ready`.

Required `yai store summary` fields:

```text
record_store_backend: lmdb
record_store_status: ready|missing|not_initialized|unavailable
record_store_path: <YAI_HOME>/store/lmdb
records_total: N
records_by_case: N
records_by_kind: N
records_by_subject: N
records_by_receipt: N
```

Required `yai store record get <record_id>` fields for a found record:

```text
schema: yai.record.v1
record_id: ...
record_kind: ...
case_ref: ...
source:
  plane: journal
  ref: ...
payload:
  summary: ...
```

Missing records return:

```text
record: not_found
```

Required list modes:

```text
yai store record list --case <case_ref> [--limit <N>]
yai store record list --kind <record_kind> [--limit <N>]
yai store record list --subject <subject_ref> [--limit <N>]
yai store record list --receipt <receipt_ref> [--limit <N>]
```

List modes report `records_total` for the matching index and print summary rows
with `record_id`, `record_kind` and `case_ref`:

```text
filter: case|kind|subject|receipt
filter_value: ...
records_total: N
limit: N
records:
- record_id: ...
  record_kind: ...
  case_ref: ...
```

Zero-result list commands return:

```text
records_total: 0
records: none
```

Subject and receipt indexes are derived from structured record fields only. If
LMDB is missing, uninitialized or unavailable, record read commands print the
record store status fields and do not synthesize records from journal.

## Journal Inspect Command

SPINE.35 adds:

```text
yai journal inspect --path <journal.jsonl> [--show-errors]
```

Required summary output:

```text
journal_path: ...
parser_policy: diagnostic
lmdb_write: no
journal_status: readable|missing|unavailable
lines_total: N
valid_entries: N
invalid_entries: N
unsupported_entries: N
duplicate_entries: N
replay_ready: yes|no
```

With `--show-errors`, diagnostics include:

```text
line: N
entry_status: invalid_json|invalid_schema|unsupported_kind|duplicate
record_id: ...
record_kind: ...
schema: ...
error_code: ...
error_message: ...
action: ...
```

Journal is replay/audit. LMDB is durable indexed record lookup. This command
does not perform replay or write LMDB.

## Journal Replay Command

SPINE.36 adds replay and SPINE.37 adds replay metadata/status:

```text
yai journal replay --path <journal.jsonl> [--dry-run]
yai journal replay-status --path <journal.jsonl>
yai journal replay-report --path <journal.jsonl>
```

Dry-run output includes:

```text
journal_replay: dry_run
journal_path: ...
journal_identity: ...
lmdb_path: <YAI_HOME>/store/lmdb
record_schema: yai.record.v1
journal_schema: yai.store.record.v0
compatibility: compatible
lines_total: N
valid_entries: N
invalid_entries: 0
records_to_write: N
cursor_line: N
would_update_cursor: yes
would_write_lmdb: yes
replay_ready: yes
```

Real replay output includes:

```text
journal_replay: completed
journal_identity: ...
record_schema: yai.record.v1
compatibility: compatible
cursor_line: N
replay_status: completed
records_seen: N
records_written: N
records_duplicate: N
records_skipped: N
record_store_status: ready
idempotent: yes|no
```

Corrupt or non-ready journals report:

```text
journal_replay: failed
replay_ready: no
records_written: 0
reason: invalid_json|invalid_schema|unsupported_kind|duplicate
```

Replay-status output includes:

```text
journal_replay_status:
journal_identity: ...
record_schema: yai.record.v1
cursor_line: N
replay_status: completed
compatibility: compatible
```

Replay materializes LMDB records. It does not change no journal fallback
behavior for store reads.

SPINE.39 freezes this surface. Future graph rebuild work must consume replay as
a stable diagnostic/materialization boundary instead of adding hidden journal
fallback behavior.

## Carrier Substrate Commands

SPINE.33A adds `yai carrier families` as an inspectable vocabulary/status
surface. SPINE.33B adds `yai carrier lanes` and `yai carrier route --family
<family>` as no-execution dispatch planning commands. These commands do not
execute a carrier and do not claim readiness for planned carrier families.

Required key output:

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
  network_http: skeleton carrier.v1
  database: skeleton carrier.v1
  repository_git: skeleton carrier.v1
  service: skeleton carrier.v1
  endpoint: skeleton carrier.v1
  socket: skeleton carrier.v1
  listener: skeleton carrier.v1
  model_provider: skeleton carrier.v1
  observation: planned
  review: skeleton carrier.v1

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

Dispatch lane inspection:

```text
yai carrier lanes

carrier_lanes:
- lane: filesystem_lane
  carrier_family: filesystem
  status: active_minimal
  ordering_policy: serial_per_case
  capacity_policy: single_inflight
  lock_policy: case_lock
  timeout_policy: normal
  retry_policy: safe_retry
  receipt_requirement: required
  execution_performed: false

- lane: process_lane
  carrier_family: process
  status: planned
  ordering_policy: serial_per_case
  capacity_policy: single_inflight
  lock_policy: target_lock
  timeout_policy: normal
  retry_policy: requires_review
  receipt_requirement: required
  execution_performed: false
```

Route inspection:

```text
yai carrier route --family filesystem
carrier_family: filesystem
lane: filesystem_lane
dispatch_status: routable
lane_status: active_minimal
execution_performed: false
receipt_requirement: required

yai carrier route --family process
carrier_family: process
lane: process_lane
dispatch_status: routable
lane_status: planned
execution_performed: false
receipt_requirement: required

yai carrier route --family unknown
carrier_family: unknown
lane: unknown_lane
dispatch_status: not_supported
lane_status: not_supported
execution_performed: false
receipt_requirement: not_available
```

Boundary language:

```text
observation is not enforcement
decision is not execution
carrier is not free tool invocation
receipt is not logging
dispatch is separate from decision
dispatch is separate from execution
```

SPINE.33C adds filesystem carrier contract inspection:

```text
yai carrier inspect filesystem

carrier: filesystem
carrier_family: filesystem
contract: carrier.v1
status: active_minimal
lane: filesystem_lane
dispatch_status: routable
supports:
  read: observed
  write: decision_required
receipt_required: yes
pre_state_observation: supported
post_state_observation: supported
evidence_capture: supported
receipt_assembly: supported
receipt_validation: supported
residue_recording: supported
guarantee_mode: interposed
```

SPINE.33D adds process carrier inspection and safe signal planning:

```text
yai carrier inspect process
carrier: process
carrier_family: process
contract: carrier.v1
status: active_minimal
lane: process_lane
supports:
  observe: observed
  signal_TERM: decision_required
  signal_KILL: test_owned_only
receipt_required: yes
guarantee_mode: interposed
platform: posix

yai process observe --pid <pid>
process_ref: process:<pid>
pid: <pid>
state: running|not_found|permission_denied|unknown
owner_scope: external_observed
carrier_family: process
outcome: observed
receipt_required: yes

yai process signal --pid <pid> --signal TERM --dry-run
op: process.signal
pid: <pid>
signal: TERM
dry_run: true
carrier_family: process
lane: process_lane
dispatch_status: routable
decision_required: true
carrier_attempted: false
expected_receipt: process_signal_receipt
```

Unsafe non-dry-run process signals to arbitrary PIDs return:

```text
carrier_attempted: false
outcome: blocked
reason: unsafe_process_target
```

SPINE.33E adds independent host observation:

```text
yai observe process --pid <pid>
observation_target: process
pid: <pid>
result: matched|not_found|permission_denied|unknown
observed_state: running|not_found|permission_denied|unknown
enforcement: none
observation_is_enforcement: false

yai observe compare-process --pid <pid> --expected stopped
observation_target: process
expected_state: stopped
observed_state: running
result: mismatch
enforcement: none
observation_is_enforcement: false
divergence_candidate: expected_stopped_but_running
silent_repair: false
```

`yai process observe` is the process carrier surface. `yai observe process` is
the independent host probe surface.

SPINE.33F adds carrier coverage visibility:

```text
yai carrier coverage
carrier_coverage:
- family: filesystem
  controlled: active_minimal
  observed: skeleton
  imported: planned
  execution_available: true
  receipt_required: yes

- family: process
  controlled: active_minimal
  observed: active_minimal
  imported: planned
  execution_available: true_limited
  receipt_required: yes

- family: database
  controlled: skeleton
  observed: skeleton
  imported: skeleton
  execution_available: false
  receipt_required: yes

yai carrier coverage --family process
family: process
controlled: active_minimal
observed: active_minimal
imported: planned
execution_available: true_limited
execution_scope: test_owned_or_safe_policy_only
receipt_required: yes
carrier_contract: carrier.v1
supported_outcomes:
- executed
- blocked
- failed
- mismatch
- observed

yai carrier coverage --family unknown
family: unknown
controlled: unsupported
observed: unsupported
imported: unsupported
execution_available: false
receipt_required: no
```

Coverage modes are `controlled`, `observed` and `imported`. They are not
outcomes. Coverage outcomes include `executed`, `blocked`, `failed`,
`mismatch`, `observed` and `not_attempted`. Skeleton carriers are visible but
must report `execution_available: false`; the carrier coverage matrix is no
fake execution.

SPINE.33G adds non-process carrier skeleton inspection:

```text
yai carrier inspect database
carrier: database
carrier_family: database
contract: carrier.v1
status: skeleton
controlled: skeleton
observed: skeleton
imported: skeleton
execution_available: false
receipt_required: yes
supports_inspect: yes
non_execution_reason: adapter_not_implemented
future_activation_wave: planned
carrier_attempted: false

yai carrier inspect model_provider
carrier: model_provider
carrier_family: model_provider
contract: carrier.v1
status: skeleton
controlled: planned
observed: planned
imported: skeleton
execution_available: false
receipt_required: yes
supports_inspect: yes
non_execution_reason: model_provider_carrier_not_implemented
future_activation_wave: SPINE.93A+

yai carrier inspect review
carrier: review
carrier_family: review
contract: carrier.v1
status: skeleton
controlled: unsupported
observed: unsupported
imported: skeleton
execution_available: false
receipt_required: yes
supports_inspect: yes
non_execution_reason: review_lane_not_implemented
future_activation_wave: planned
```

Skeleton inspect is available for `network_http`, `database`,
`repository_git`, `service`, `endpoint`, `socket`, `listener`,
`model_provider` and `review`. It does not execute carrier effects.

SPINE.33H adds carrier outcome posture testing:

```text
yai carrier outcome-test --family database --outcome blocked
family: database
mode: controlled
requested_outcome: blocked
effective_outcome: blocked
carrier_status: skeleton
execution_available: false
execution_performed: false
carrier_attempted: false
receipt_required: yes
receipt_posture: simulated
divergence_candidate: none
reason: skeleton_carrier_no_execution

yai carrier outcome-test --family repository_git --mode observed --outcome mismatch
family: repository_git
mode: observed
requested_outcome: mismatch
effective_outcome: mismatch
carrier_status: skeleton
execution_available: false
execution_performed: false
carrier_attempted: false
receipt_required: yes
receipt_posture: simulated
divergence_candidate: generated
reason: simulated_mismatch_posture

yai carrier outcome-test --family unknown --outcome blocked
family: unknown
mode: controlled
requested_outcome: blocked
effective_outcome: not_attempted
carrier_status: unsupported
execution_available: false
execution_performed: false
carrier_attempted: false
receipt_required: no
receipt_posture: none
divergence_candidate: none
reason: unsupported_carrier_family
```

Use `yai carrier outcome-test` to inspect outcome posture without executing
skeleton carriers. Invalid outcomes return:

```text
error: unsupported_outcome
```

SPINE.33I adds carrier receipt / divergence reconciliation:

```text
yai carrier reconcile-outcome --scenario denied_but_attempted
scenario: denied_but_attempted
decision: deny
dispatch: blocked
carrier_outcome: executed
receipt_present: yes
observation: matched
carrier_attempted: true
execution_performed: true
divergence_candidate: denied_but_attempted
severity: critical
result: inconsistent

yai carrier reconcile-outcome --scenario clean_blocked
scenario: clean_blocked
decision: deny
dispatch: blocked
carrier_outcome: blocked
receipt_present: yes
carrier_attempted: false
execution_performed: false
divergence_candidate: none
severity: info
result: consistent
```

Use `yai carrier reconcile-outcome` to check whether outcome posture is
coherent against decision, dispatch, receipt posture and observation result. It
does not execute carriers.

SPINE.33L adds provider runtime / LAN target surface v0:

| Primitive | View | Command | Lab test | Docs |
|---|---|---|---|---|
| device registry | JSONL registry under `YAI_HOME/config/devices.jsonl` | `yai device list` | `target/debug/yai device list` | `provider-runtime-lan-target-surface.md`, `testing.md` |
| device add | declared runtime target | `yai device add --id <id> --name <name> --host <host> --port <port> --target lan` | `target/debug/yai device add --id workstation --name Workstation --host 192.168.1.50 --port 8777 --target lan` | `provider-runtime-lan-target-surface.md` |
| device inspect | target/trust/supervisor posture | `yai device inspect <device_id>` | `target/debug/yai device inspect workstation` | `provider-runtime-lan-target-surface.md` |
| trusted device posture | registry trust update | `yai device trust <device_id>` | `target/debug/yai device trust workstation` | `provider-runtime-lan-target-surface.md` |
| provider runtime targets | runtime target vocabulary | `yai provider runtime status`, `yai provider targets` | `target/debug/yai provider runtime status` | `provider-runtime-lan-target-surface.md` |
| provider start dry-run | no-execution provider start plan | `yai provider start --dry-run --target <target> ...` | `target/debug/yai provider start --dry-run --target lan --device workstation --kind ds4 --model deepseek-v4-flash` | `provider-runtime-lan-target-surface.md` |
| provider logs | reserved log/run/plan paths | `yai provider logs-path` | `target/debug/yai provider logs-path` | `provider-runtime-lan-target-surface.md` |
| model catalog | scan posture only | `yai model catalog status` | `target/debug/yai model catalog status` | `provider-runtime-lan-target-surface.md` |
| model runtime | routing/retrieval/decoding status | `yai model runtime status` | `target/debug/yai model runtime status` | `provider-runtime-lan-target-surface.md` |

Required `yai device list` empty output:

```text
devices: none
device_registry: <YAI_HOME>/config/devices.jsonl
```

Required LAN dry-run blocked output:

```text
provider_start_plan:
  target: lan
  device_id: workstation
  status: blocked
  reason: device_not_trusted
  execution_performed: false
```

Required trusted LAN dry-run output:

```text
provider_start_plan:
  target: lan
  device_id: workstation
  provider_kind: ds4
  model: deepseek-v4-flash
  status: planned
  action: dry_run_only
  would_contact_device: true
  would_start_provider: true
  execution_performed: false
  receipt_required: yes
```

Required model runtime posture:

```text
model_runtime:
  provider_supervision: planned
  model_routing: planned
  context_compiler: planned
  retrieval_hnsw: planned
  decoding_acceleration: planned
  fallback: normal_decoding
```

SPINE.33L does not start provider processes, contact LAN devices, scan model
files, probe GPUs, implement HNSW or implement MTP.

Reserved future commands for the retrieval and runner roadmap rebase:

```text
yai retrieval status
yai retrieval build
yai retrieval query
yai retrieval candidates
yai context compile --dry-run
yai context inspect <frame/ref>
yai model-runtime inspect
yai model-runtime capabilities
yai model-runtime invoke --dry-run
yai model-runtime benchmark --decoding normal|mtp
```

These commands are not implemented by SPINE.33J or SPINE.33K. They are reserved
so future roadmap work has an explicit command-surface placeholder without
implying HNSW, vector DB, embedding, context compiler, MTP, DS4 or model runner
support.

SPINE.33M adds no runtime command surface. It corrects the vocabulary for
future graph, retrieval and context compiler commands:

```text
LMDB stores records.
Ladybug stores relations.
DuckDB stores facts.
RuntimeGraph computes over the active case.
HNSW finds candidate nodes.
Context Compiler renders controlled views.
```

Future command surfaces may inspect Data Context Runtime state,
RuntimeGraph working sets and HNSW Candidate -> RuntimeGraph Expansion, but
SPINE.33M implements no `yai runtime-graph`, `yai retrieval` or `yai context`
command.

SPINE.40 adds the first graph schema/status command surface:

```text
yai graph schema
yai graph runtime-status
```

`yai graph schema` prints graph node kinds and edge kinds plus
the active-minimal relation write path. `yai graph runtime-status` reports
RuntimeGraph as `planned`, with role `in_memory_active_case_working_set`,
durable truth `graph_persistence`, HNSW as `future_candidate_index` and Context
Compiler as `future_consumer`.

Graph persistence owns durable typed relations. RuntimeGraph is the in-memory active case working set.
HNSW finds candidate nodes and HNSW is not graph truth.
Context Compiler renders controlled views. Projection does not disappear.
Persistent truth on disk. Computational shape in memory.

SPINE.41 adds the first graph relation write commands:

```text
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref> [--limit <N>]
```

`yai graph materialize` derives `yai.graph_relation.v1` from LMDB records and
writes to `lmdb_graph_relations_v0`. It reports `relation_id` provenance,
`source_record_id`, `relations_written`, `relations_duplicate` and
`relations_skipped`. The required derived edges include
`decision_controls_attempt` and `receipt_records_effect`.

`yai graph relations` lists relations by case. RuntimeGraph remains planned.
Ladybug integration remains future.

SPINE.42 activates the first RuntimeGraph working set command surface:

```text
yai graph runtime-status
yai graph runtime-load --case <case_ref>
yai graph runtime-summary --case <case_ref>
```

The working set is per-command ephemeral and the resident service is planned.

`yai graph runtime-status` now reports `status: active_minimal`,
`working_set: per_command_ephemeral`, `resident_service: planned`,
`source: graph_relations`, `durable_truth: graph_persistence`,
`hnsw: future_candidate_index` and `context_compiler: future_consumer`.

`yai graph runtime-load` loads the case-local in-memory working set from graph
relations and prints `nodes_total`, `edges_total`, `outgoing_index_entries`,
`incoming_index_entries`, `dirty: no`, `stale: no` and `resident: false`.
Graph persistence is durable truth. HNSW remains future candidate index.
Context Compiler remains future consumer.

## Projection Commands

| Primitive | View | Command | Lab test | Docs |
|---|---|---|---|---|
| projection summary | journal-derived projection summary | `yai projection summary --journal <path>` | smoke journal path | `testing.md` |
| projection inspect | projection details and freshness policy | `yai projection inspect --journal <path> --consumer <consumer>` | `yai projection inspect --journal <journal> --consumer model` | `data-plane-roadmap.md` |
| prompt dry run | model-visible projection warning surface | `yai prompt --dry-run --once <text> --journal <path>` | daemon core-loop smoke journal | `testing.md` |

Projection commands are still journal-backed in this phase. Hot state provides
freshness context, not durable projection truth.

Required `yai projection inspect` freshness fields:

```text
projection_freshness: fresh|stale|unknown|rebuilding
stale_reason: ...
freshness_policy: usable|refresh_recommended|refresh_required|blocked_for_model|unknown
consumer: model|operator|audit|debug|agent
freshness_source: hot_state|projection_record|journal|unknown
```

If `projection inspect` cannot correlate the journal case with active hot
state, it reports journal/projection-derived freshness. It must not fake a
hot-state source.

## Pack / Doctrine Checks

| Primitive | View | Command | Lab test | Docs |
|---|---|---|---|---|
| pack doctrine | required pack doctrine doc/phrases | `make check-pack-doctrine` | `make check-pack-doctrine` | `pack-format.md`, `pack-roadmap.md` |
| filesystem-loop lab pack fixture files | staged pack material before provider attach | shell copy/JSON validation in lab | `cp labs/filesystem-loop/pack-fixture/policies/*.json "$YAI_RUN/pack-fixture/policies"/` | `labs/filesystem-loop/runbook.md` |
| pack-derived case residue | materialized policy/projection/authority records | `yai daemon run-filesystem-loop --socket <path>` | `grep subject:policy-pack "$JOURNAL"` | `labs/filesystem-loop/runbook.md` |
| active docs | canonical engineering doc set | `make check-docs` | `make check-docs` | `README.md` |

Pack runtime commands are not implemented yet. SPINE.21 only provides pack
doctrine and guards. The filesystem loop lab stages pack fixture files and
the daemon loop materializes their current posture into journal records for the
filesystem-loop lab case. Do not document a generic pack runtime command until the pack runtime exists.

## Foundation / Layout Checks

| Primitive | View | Command | Lab test | Docs |
|---|---|---|---|---|
| source layout | no old active roots | `make check-layout` | `make check-layout` | `filesystem-target.md` |
| source surface | no venv roots, no README-only source roots, transitional C shims marked | `make check-source-surface-clean` | `make check-source-surface-clean` | `source-surface.md` |
| foundation freeze | freeze guard | `make check-foundation-freeze` | `make check-foundation-freeze` | `current-status.md` |
| full docs guard | active doc requirements | `make check-docs` | `make check-docs` | `README.md` |
| archive hygiene | historical banner on archive records | `make check-archive-historical-records` | `make check-archive-historical-records` | `work/archive/legacy-docs/README.md` |
| full check | layout, docs, build, smoke | `make check` | `make check` | `testing.md` |

`make check-source-surface-clean` is integrated through `make check-layout`.
`make check-foundation-freeze` is integrated through `make check-docs`, and
therefore through `make check`.

## Install Commands

Manual install matrix:

```sh
rm -rf /tmp/yai-install-test /tmp/yai-home-test
make install-local PREFIX=/tmp/yai-install-test YAI_HOME=/tmp/yai-home-test
PATH=/tmp/yai-install-test/bin:$PATH yai --version
PATH=/tmp/yai-install-test/bin:$PATH yai info
PATH=/tmp/yai-install-test/bin:$PATH yai doctor
PATH=/tmp/yai-install-test/bin:$PATH yai store status
PATH=/tmp/yai-install-test/bin:$PATH yai store summary
PATH=/tmp/yai-install-test/bin:$PATH yai store record list --kind receipt --limit 10
PATH=/tmp/yai-install-test/bin:$PATH yai store record list --subject subject:filesystem-sandbox --limit 20
PATH=/tmp/yai-install-test/bin:$PATH yai store record list --receipt receipt:new12-fs-write --limit 10
PATH=/tmp/yai-install-test/bin:$PATH yai store record get rec:new12-min-receipt
PATH=/tmp/yai-install-test/bin:$PATH yai hot status
/tmp/yai-install-test/bin/yaid --version
make uninstall-local PREFIX=/tmp/yai-install-test
test ! -e /tmp/yai-install-test/bin/yai
test ! -e /tmp/yai-install-test/bin/yaid
```

`uninstall-local` removes installed binaries. It does not delete `YAI_HOME`.

## Manual Validation Recipes

Daemon and hot-state recipe:

```sh
mkdir -p /tmp/yai-home-test/run
/tmp/yai-install-test/bin/yaid \
  --socket /tmp/yai-home-test/run/yaid.sock \
  --foreground &
DAEMON_PID=$!

PATH=/tmp/yai-install-test/bin:$PATH \
  yai daemon status --socket /tmp/yai-home-test/run/yaid.sock

PATH=/tmp/yai-install-test/bin:$PATH \
  yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock

PATH=/tmp/yai-install-test/bin:$PATH yai hot status

PATH=/tmp/yai-install-test/bin:$PATH \
  yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock

wait $DAEMON_PID
```

Expected key output:

```text
hot_state: active
snapshot: /tmp/yai-home-test/run/hot-state.json
schema: yai.hot_state.v1
case_session: active
case_context: active
projection_policy: usable
freshness_policy: usable
```

Snapshot edge cases:

```sh
rm -f /tmp/yai-home-test/run/hot-state.json
PATH=/tmp/yai-install-test/bin:$PATH yai hot status

printf '{broken\n' > /tmp/yai-home-test/run/hot-state.json
PATH=/tmp/yai-install-test/bin:$PATH yai hot status
```

Expected key output:

```text
hot_state: unavailable
reason: missing_snapshot
hot_state: unavailable
reason: invalid_snapshot
```

Automated retroactive coverage:

```sh
make smoke-spine24a
make smoke-spine27
make smoke-spine29
make smoke-spine30
make check-hot-state-freeze
make check-lmdb-record-plane-doctrine
```

## RuntimeGraph Rebuild Commands

SPINE.43 adds RuntimeGraph rebuild. RuntimeGraph rebuild runs the durable
chain from journal or existing graph relations into a per-command RuntimeGraph
working set and writes `yai.runtime_graph_rebuild_report.v1`.

```bash
yai graph rebuild --case <case_ref> --from journal --path <journal.jsonl>
yai graph rebuild --case <case_ref> --from graph-relations
yai graph rebuild-report --case <case_ref>
yai graph runtime-summary --case <case_ref>
```

`yai graph rebuild --from journal` runs journal replay to LMDB, graph
materialization and RuntimeGraph load. `yai graph rebuild --from
graph-relations` loads from existing graph relations. RuntimeGraph is not
durable truth. RuntimeGraph is not durable truth and resident service planned
remains the runtime posture.

Required output:

```text
runtime_graph_rebuild:
journal_replay_status:
graph_materialize_status:
runtime_graph_status:
nodes_total:
edges_total:
rebuild_status:
report_schema: yai.runtime_graph_rebuild_report.v1
```

## RuntimeGraph Query Commands

SPINE.44 adds bounded traversal diagnostics over the per-command RuntimeGraph
working set. RuntimeGraph is not a generic graph database.

```bash
yai graph fanout --case <case_ref> --node <ref> [--edge-kind <kind>] [--limit <N>]
yai graph fanin --case <case_ref> --node <ref> [--edge-kind <kind>] [--limit <N>]
yai graph neighborhood --case <case_ref> --node <ref> --depth 1 [--edge-kind <kind>]
yai graph path --case <case_ref> --from <ref> --to <ref> --max-depth 4
```

Fanout, fanin and neighborhood accept an edge-kind filter. Neighborhood and
causal path diagnostics are bounded traversal only; max-depth clamps prevent
unbounded search. Plain output remains parseable. Color-aware graph inspection
is future visual doctrine for console/studio views.

The active lab path is `labs/filesystem-loop`.

Guard vocabulary: RuntimeGraph is not a generic graph database; fanout, fanin,
neighborhood, bounded traversal, causal path, max-depth, edge-kind filter,
color-aware graph inspection, plain output remains parseable.

## Operator Review Commands

SPINE.44A adds the first active review/deferred/quarantine loop. A filesystem
write may stop at `require_review`, appear as `pending_operator`, and resolve
through an operator reviewer authority.

```bash
yai daemon run-filesystem-review-loop --socket <socket>
yai control pending --case <case_ref>
yai control show <review_id>
yai control review --case <case_ref> --interactive
yai control watch --case <case_ref> --interval-ms 500 --max-events 5
yai control wait <review_id> --timeout 1
yai control approve <review_id> --reason "<reason>"
yai control deny <review_id> --reason "<reason>"
yai control defer <review_id> --reason "<reason>"
yai control quarantine <review_id> --reason "<reason>"
```

For automation and smoke tests, `yai daemon run-filesystem-review-loop` prints
`review_required: yes`, `status: pending_operator`, `carrier_attempted:
false`, `execution_performed: false` and `next_commands`, then exits.

SPINE.44B adds the CLI interaction surface. `control pending` includes
`next_commands` for automation. `control show` exposes the operation target,
policy reason and authority boundary. `control review --interactive` prompts
only when a TTY is present and returns `not_a_tty` otherwise. `control watch`
polls state changes and `control wait` exits on resolution or timeout.

`subject:linenoise-terminal is prompt surface`. Operator reviewer authority is
separate and local-dev scoped. Deny, defer and quarantine keep
`carrier_attempted: false` and `execution_performed: false`.
Authority boundary: `subject:operator-reviewer is review authority`.

The active lab path is `labs/filesystem-loop`.

Guard vocabulary: control pending, control show, control review --interactive,
control watch, control wait, next_commands, not_a_tty, pending_operator,
approve, deny, defer, quarantine, carrier_attempted: false,
execution_performed: false, subject:linenoise-terminal is prompt surface,
subject:operator-reviewer is review authority, labs/filesystem-loop.

## Review Loop Test Matrix Commands

SPINE.44C adds no new command semantics. It aligns the command surface with
the review matrix in `labs/filesystem-loop` and the model posture notes in
`labs/filesystem-loop`.

Matrix commands:

```bash
yai daemon run-filesystem-review-loop --socket <socket>
yai control pending --case <case_ref>
yai control show <review_id>
yai control approve <review_id> --reason "..."
yai control deny <review_id> --reason "..."
yai control defer <review_id> --reason "..."
yai control quarantine <review_id> --reason "..."
yai control wait <review_id> --timeout <seconds>
yai control watch --case <case_ref> --max-events <N>
```

The matrix proves `pending_operator`, `next_commands`, `wait timeout`, bounded
`watch`, approve, deny, defer and quarantine. Deny, defer and quarantine keep
`carrier_attempted: false` and `execution_performed: false`.

Model-facing posture remains separate: model proposal observed, model cannot
approve, and automatic proposed-op gate import is future work.

Review matrix guard: model proposal observed; model cannot approve; automatic
proposed-op gate import is future work.

## SPINE.45 Graph + RuntimeGraph Freeze Commands

SPINE.45 freezes the graph command surface without adding commands:

```bash
yai graph schema
yai graph runtime-status
yai graph materialize --case <case_ref>
yai graph relations --case <case_ref>
yai graph runtime-load --case <case_ref>
yai graph runtime-summary --case <case_ref>
yai graph rebuild --case <case_ref> --from journal --path <journal.jsonl>
yai graph rebuild --case <case_ref> --from graph-relations
yai graph rebuild-report --case <case_ref>
yai graph fanout --case <case_ref> --node <ref> [--edge-kind <kind>] [--limit <N>]
yai graph fanin --case <case_ref> --node <ref> [--edge-kind <kind>] [--limit <N>]
yai graph neighborhood --case <case_ref> --node <ref> [--depth <N>] [--limit <N>]
yai graph path --case <case_ref> --from <ref> --to <ref> [--max-depth <N>]
```

The frozen output includes `yai.graph_relation.v1`,
`yai.runtime_graph_rebuild_report.v1`, fanout, fanin, neighborhood, causal
path, bounded traversal and edge-kind filter. Plain output remains parseable;
color-aware graph inspection remains documented visual doctrine.

RuntimeGraph is not durable truth. HNSW future, Context Compiler future and
Ladybug future persistence integration remain outside this block.
`review_request`, `review_decision` and `control_pending` are graph-visible
when actual refs exist. Approve is graph/query visible; deny, defer and
quarantine preserve no-execution posture.

## SPINE.45A Documentation Root Canon Commands

SPINE.45A adds no `yai` commands. It adds one repository validation target:

```bash
make check-doc-root-canon
```

`make check-docs` includes `check-doc-root-canon`. The target verifies that
`docs/` is curated architecture documentation only, `labs/` is the lab root and
`work/` owns spines, waves, inventories and archives.

## SPINE.45B Case Runtime Semantics Roadmap Commands

SPINE.45B adds no `yai` commands. It adds one roadmap/doctrine validation
target:

```bash
make check-case-runtime-semantics-roadmap
```

`make check-docs` includes `check-case-runtime-semantics-roadmap`. The target
verifies bitemporal fact doctrine, native/attached/federated retrieval,
ContextDelta, CaseModelSession, KV/cache boundary and temporal graph revision
roadmap terms.

## SPINE.46 DuckDB Fact Plane Commands

SPINE.46 adds the initial analytical fact-plane surface:

```bash
yai facts status
yai facts schema
yai facts init
make check-duckdb-fact-plane
make smoke-spine46
```

`yai facts status` reports the DuckDB backend, the
`YAI_HOME/store/facts/yai-facts.duckdb` path, schema `yai.fact.v1`,
`bitemporal: yes`, `facts_are_truth: false` and `operational_truth: false`.
Before initialization it reports `status: not_initialized`; after
initialization it reports `status: ready`, `tables: 12` and
`facts_extracted: 0`.

`yai facts schema` lists the planned fact tables:

```text
fact_receipt
fact_decision
fact_projection
fact_carrier_outcome
fact_divergence
fact_replay
fact_runtime_graph
fact_model_behavior
fact_policy_outcome
fact_memory_quality
fact_retrieval_quality
fact_provider_runtime
```

It also lists common bitemporal/revision columns:
`transaction_time`, `valid_time_start`, `valid_time_end`, `known_at`,
`status`, `revision_of`, `superseded_by` and `retracted_by`.

`yai facts init` creates the DuckDB file and schema tables only. Facts are not
truth. SPINE.46 has No fact extraction; `facts_extracted: 0` is the required
posture until SPINE.47.

## SPINE.47 Receipt / Decision / Projection Fact Commands

SPINE.47 adds the first fact extraction commands:

```bash
yai facts extract --case <case_ref> --kind receipt
yai facts extract --case <case_ref> --kind decision
yai facts extract --case <case_ref> --kind projection
yai facts extract --case <case_ref> --kind core
yai facts summary --case <case_ref>
make check-receipt-decision-projection-facts
make smoke-spine47
```

`core` means receipt + decision + projection. SPINE.48 expands `all` to core +
behavior.

Fact extraction is derivation, not migration. Records remain truth in LMDB.
Facts are not truth. Deterministic fact IDs use:

```text
fact:<kind>:<source_record_id>
```

Extraction is idempotent extraction; a second run reports duplicates. The row
shape populates `transaction_time`, `valid_time_start`, `valid_time_end`,
`known_at` and revision fields. valid_time_end sentinel: 0 means open-ended.
No fact revision or supersession is implemented yet.

## SPINE.48 Model Behavior / Policy Outcome Fact Commands

SPINE.48 extends `yai facts extract` with behavior extraction:

```bash
yai facts extract --case <case_ref> --kind model_behavior
yai facts extract --case <case_ref> --kind policy_outcome
yai facts extract --case <case_ref> --kind behavior
yai facts extract --case <case_ref> --kind all
yai facts summary --case <case_ref>
make check-model-behavior-policy-facts
make smoke-spine48
```

`core` remains receipt + decision + projection. `behavior` means
model_behavior + policy_outcome. `all` means core + behavior. Guard
vocabulary: kind model_behavior, kind policy_outcome, kind behavior.

`facts summary` includes `fact_model_behavior` and `fact_policy_outcome`.
Model behavior facts measure model output posture; policy outcome facts measure
policy/control posture. Facts are not truth. model proposal is not execution.
model cannot approve. automatic proposed-op gate import is future work.
`authority_overclaim`, `unsupported_claim`, `review_required` and
`policy_outcome` are analytical fields only. No LLM-based classification is
used.

## SPINE.49 Memory / Divergence / Carrier Fact Commands

SPINE.49 extends `yai facts extract` with operational extraction:

```bash
yai facts extract --case <case_ref> --kind carrier_outcome
yai facts extract --case <case_ref> --kind divergence
yai facts extract --case <case_ref> --kind memory_quality
yai facts extract --case <case_ref> --kind operational
yai facts extract --case <case_ref> --kind all
yai facts summary --case <case_ref>
make check-memory-divergence-carrier-facts
make smoke-spine49
```

`operational` means carrier_outcome + divergence + memory_quality. `all` means
core + behavior + operational. Guard vocabulary: kind carrier_outcome, kind
divergence, kind memory_quality, kind operational.

`facts summary` includes `fact_carrier_outcome`, `fact_divergence` and
`fact_memory_quality`. carrier facts measure carrier posture. divergence facts
are not reconcile action. memory facts are not memory. facts are not truth.
Extraction is bitemporal and idempotent extraction.
