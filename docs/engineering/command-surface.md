# Command Surface

This is the active operator command index for `yai`.

Rule:

```text
A primitive that cannot be inspected is not operational yet.
```

This file maps SPINE.20-SPINE.32 primitives to their current view, command,
manual test and documentation surface. It does not define new core semantics.
The operator-facing runbook is `docs/manuals/manual-filesystem-loop-validation.md`
with notebook companions `docs/manuals/manual-filesystem-loop-validation.ipynb`
and `docs/manuals/manual-filesystem-loop-validation.it.ipynb`.

## Runtime Commands

| Primitive | View | Command | Manual test | Docs |
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

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| `yaid` binary | daemon version | `yaid --version` | `/tmp/yai-install-test/bin/yaid --version` | `testing.md` |
| local socket | daemon foreground endpoint | `yaid --socket <path> --foreground` | `yaid --socket /tmp/yai-home-test/run/yaid.sock --foreground` | `filesystem-target.md` |
| daemon liveness | status response | `yai daemon status --socket <path>` | `yai daemon status --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| daemon metadata | daemon info response | `yai daemon info --socket <path>` | `yai daemon info --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| minimum loop | daemon-backed control loop | `yai daemon run-minimum-loop --socket <path>` | `yai daemon run-minimum-loop --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| filesystem loop | daemon-backed filesystem loop | `yai daemon run-filesystem-loop --socket <path>` | `yai daemon run-filesystem-loop --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |
| shutdown | daemon stop request | `yai daemon shutdown --socket <path>` | `yai daemon shutdown --socket /tmp/yai-home-test/run/yaid.sock` | `testing.md` |

## Hot State Commands

| Primitive | View | Command | Manual test | Docs |
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

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| record store | LMDB record-plane readiness | `yai store status` | `target/debug/yai store status` | `lmdb-record-plane.md`, `data-plane-roadmap.md` |
| record store summary | LMDB aggregate write-path counts | `yai store summary` | `target/debug/yai store summary` | `lmdb-record-plane.md`, `testing.md` |
| record get | LMDB id lookup | `yai store record get <record_id>` | `target/debug/yai store record get rec:new12-min-receipt` | `lmdb-record-plane.md`, `testing.md` |
| record list by case | LMDB case index scan | `yai store record list --case <case_ref> [--limit <N>]` | `target/debug/yai store record list --case case:new12-daemon --limit 10` | `lmdb-record-plane.md`, `testing.md` |
| record list by kind | LMDB kind index scan | `yai store record list --kind <record_kind> [--limit <N>]` | `target/debug/yai store record list --kind receipt --limit 10` | `lmdb-record-plane.md`, `testing.md` |
| record list by subject | LMDB subject index scan | `yai store record list --subject <subject_ref> [--limit <N>]` | `target/debug/yai store record list --subject subject:filesystem-sandbox --limit 20` | `lmdb-record-plane.md`, `testing.md` |
| record list by receipt | LMDB receipt index scan | `yai store record list --receipt <receipt_ref> [--limit <N>]` | `target/debug/yai store record list --receipt receipt:new12-fs-write --limit 10` | `lmdb-record-plane.md`, `testing.md` |
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

`yai store status` is the readiness view because `store` already names the
durable data root and LMDB is the record-plane backend under it. SPINE.30 adds
`yai store summary` for aggregate write-path validation. SPINE.31 adds
read-only `yai store record get` and `yai store record list` over the id, case
and kind indexes. SPINE.32 extends list with subject and receipt filters.
SPINE.33 freezes the line-oriented operator output shape before the record-plane
freeze. Future subcommands can extend the same surface with
`yai store rebuild status`.

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
- model_provider
- observation
- review

current_status:
  filesystem: implemented_minimal
  process: planned

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

## Projection Commands

| Primitive | View | Command | Manual test | Docs |
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

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| pack doctrine | required pack doctrine doc/phrases | `make check-pack-doctrine` | `make check-pack-doctrine` | `pack-format.md`, `pack-roadmap.md` |
| manual policy pack fixtures | staged pack material before provider attach | shell copy/JSON validation in manual | `cp docs/manuals/examples/filesystem-loop/policy-packs/*.json "$YAI_RUN/policy-packs"/` | `manual-filesystem-loop-validation.md` |
| pack-derived case residue | materialized policy/projection/authority records | `yai daemon run-filesystem-loop --socket <path>` | `grep subject:policy-pack "$JOURNAL"` | `manual-filesystem-loop-validation.md` |
| active docs | canonical engineering doc set | `make check-docs` | `make check-docs` | `README.md` |

Pack runtime commands are not implemented yet. SPINE.21 only provides pack
doctrine and guards. The filesystem loop manual stages policy pack fixtures and
the daemon loop materializes their current posture into journal records for the
manual case. Do not document a `yai pack` command until the pack runtime exists.

## Foundation / Layout Checks

| Primitive | View | Command | Manual test | Docs |
|---|---|---|---|---|
| source layout | no old active roots | `make check-layout` | `make check-layout` | `filesystem-target.md` |
| source surface | no venv roots, no README-only source roots, transitional C shims marked | `make check-source-surface-clean` | `make check-source-surface-clean` | `source-surface.md` |
| foundation freeze | freeze guard | `make check-foundation-freeze` | `make check-foundation-freeze` | `current-status.md` |
| full docs guard | active doc requirements | `make check-docs` | `make check-docs` | `README.md` |
| archive hygiene | historical banner on archive records | `make check-archive-historical-records` | `make check-archive-historical-records` | `docs/archive/README.md` |
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
