# Filesystem Loop Validation Output

Date: 2026-05-24
Repository: YAI
Run source:

- docs/manuals/manual-filesystem-loop-validation.md
- docs/manuals/manual-filesystem-loop-validation.ipynb

Scope:

- Command-surface baseline without LAN provider traffic.
- Local build and install.
- Local daemon lifecycle.
- Filesystem loop materialization.
- Hot-state and projection inspection.
- Snapshot missing/corrupt edge cases.
- Targeted smoke checks.

Notes:

- Jupyter/nbconvert were not installed in this environment, so this report is a
  shell execution of the notebook/runbook baseline rather than a saved executed
  notebook.
- Provider prompt cells were not run because no LAN provider endpoint was
  configured for this execution.


## Environment

```text
ROOT=/home/mothx/COMPUTER_SCIENCE/DEV_CODE/YAI/yai
PREFIX=/tmp/yai-install-test
YAI_HOME=/tmp/yai-home-test
YAI_SOCKET=/tmp/yai-home-test/run/yaid.sock
PATH=/home/mothx/.codex/tmp/arg0/codex-arg0LxO1K8:/home/mothx/.cargo/bin:/usr/local/sbin:/usr/local/bin:/usr/bin:/opt/cuda/bin:/var/lib/flatpak/exports/bin:/usr/lib/jvm/default/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl:/var/lib/snapd/snap/bin:/home/mothx/.vscode-insiders/extensions/openai.chatgpt-26.5506.21252-linux-x64/bin/linux-x86_64

master
eece96a
```

## make info

```sh
$ make info
```

```text
yai: local AI operational control core
status: SPINE.32 LMDB Case / Subject / Receipt Indexes
completed: SPINE.20 Local Runtime Layout through SPINE.32 LMDB Case / Subject / Receipt Indexes
next: SPINE.33 LMDB CLI + Manual Validation
target-layout: include/ system/ engine/ cmd/
runtime-home: YAI_HOME=/home/mothx/.yai socket=/home/mothx/.yai/run/yaid.sock
hot-state: /home/mothx/.yai/run/hot-state.json
record-store: /home/mothx/.yai/store/lmdb
pack-doctrine: active docs/engineering/pack-format.md
foundation-freeze: filesystem closed; runtime layout exists; active docs compact; extraction contract active
data-spine-c: transitional keep_temporarily
engine-bridge: active
lib: removed
daemon: moved to cmd/yaid + system/daemon
crates: removed
ctl: removed
install-local: active PREFIX=/home/mothx/.local YAI_HOME=/home/mothx/.yai

```

Exit status: `0`


## Clean runtime and build

```sh

set -eu
rm -rf '/tmp/yai-install-test' '/tmp/yai-home-test'
make build
target/debug/yai --version
target/debug/yai info
target/debug/yai doctor
target/debug/yai hot status || true
build/yaid --version

```

```text
CARGO_TARGET_DIR=target cargo build --manifest-path engine/Cargo.toml -p yai-engine
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.01s
CARGO_TARGET_DIR=target cargo build --manifest-path engine/Cargo.toml --workspace
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.00s
CARGO_TARGET_DIR=target cargo test --manifest-path engine/Cargo.toml --workspace
    Finished `test` profile [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/lib.rs (target/debug/deps/yai_core_engine-64d462f22a0113a9)

running 11 tests
test tests::append_record_and_build_projection_summary ... ok
test tests::build_authority_projection_summary ... ok
test tests::build_control_projection_summary ... ok
test tests::build_filesystem_projection_summary ... ok
test tests::build_graph_projection_summary ... ok
test tests::build_reconcile_projection_summary ... ok
test tests::build_memory_projection_summary ... ok
test tests::build_hardened_projection_summary ... ok
test tests::parse_jsonl_and_build_projection_summary ... ok
test tests::query_records_by_kind_and_case ... ok
test tests::rust_engine_ffi_append_and_count ... ok

test result: ok. 11 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

     Running unittests src/lib.rs (target/debug/deps/yai_core_engine_sys-24677f9bc7566579)

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

   Doc-tests yai_core_engine

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

   Doc-tests yai_core_engine_sys

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

CARGO_TARGET_DIR=target cargo build --manifest-path cmd/yai/Cargo.toml
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.00s
CARGO_TARGET_DIR=target cargo test --manifest-path cmd/yai/Cargo.toml
    Finished `test` profile [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/main.rs (target/debug/deps/yai-7985b15b2fa747b6)

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

yai 0.0.0-newcore
yai: technical YAI control command
status: SPINE.32 LMDB Case / Subject / Receipt Indexes
ownership: Rust client over C-defined core primitives
daemon_ipc: local Unix socket with daemon-backed loop v0
canonical_daemon: yaid
runtime_layout: YAI_HOME local runtime v0
foundation_freeze: filesystem_runtime_layout
hot_state: YAI_HOME/run/hot-state.json live cache v0
record_store: YAI_HOME/store/lmdb planned LMDB lookup plane
journal_inspection: file-based JSONL v0
control_inspection: journal-derived summary
yai doctor: ok
public_semantics: C ABI + core docs
rust_role: yai client and operational data engine skeleton
journal_mode: file debug only
binary_path: /home/mothx/COMPUTER_SCIENCE/DEV_CODE/YAI/yai/target/debug/yai
yaid_path: not found on PATH
yaid_found: missing
yai_version: 0.0.0-newcore
YAI_HOME: /home/mothx/.yai
YAI_HOME_status: ok
run_dir: /home/mothx/.yai/run missing
store_dir: /home/mothx/.yai/store missing
log_dir: /home/mothx/.yai/log missing
tmp_dir: /home/mothx/.yai/tmp missing
cases_dir: /home/mothx/.yai/cases missing
sockets_dir: /home/mothx/.yai/sockets missing
config_dir: /home/mothx/.yai/config missing
env_file: not found
PATH_status: warning current binary dir not on PATH
daemon_socket_default: /home/mothx/.yai/run/yaid.sock
socket_default_status: not_present
hot_state_path: /home/mothx/.yai/run/hot-state.json
hot_state_status: unavailable
hot_state_schema_status: missing
hot_state_readable: no
record_store_path: /home/mothx/.yai/store/lmdb
record_store_status: missing
record_store_backend: lmdb
runtime_layout_status: incomplete
hot_state: unavailable
reason: missing_snapshot
snapshot: /home/mothx/.yai/run/hot-state.json
snapshot_path: /home/mothx/.yai/run/hot-state.json
snapshot_status: unavailable
schema: unknown
case_session: unknown
case_world: unknown
case_context: unknown
active_thread: unknown
participant_view: unknown
projection: unknown
projection_policy: unknown
freshness_policy: unknown
stale_reason: unknown
yaid 0.0.0-newcore

```

Exit status: `0`


## Install local runtime

```sh

set -eu
make install-local PREFIX='/tmp/yai-install-test' YAI_HOME='/tmp/yai-home-test'
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai --version
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai info
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai doctor
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai hot status || true
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yaid --version

```

```text
CARGO_TARGET_DIR=target cargo build --manifest-path engine/Cargo.toml -p yai-engine
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.00s
CARGO_TARGET_DIR=target cargo build --manifest-path engine/Cargo.toml --workspace
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.00s
CARGO_TARGET_DIR=target cargo test --manifest-path engine/Cargo.toml --workspace
    Finished `test` profile [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/lib.rs (target/debug/deps/yai_core_engine-64d462f22a0113a9)

running 11 tests
test tests::append_record_and_build_projection_summary ... ok
test tests::build_control_projection_summary ... ok
test tests::build_authority_projection_summary ... ok
test tests::build_filesystem_projection_summary ... ok
test tests::build_graph_projection_summary ... ok
test tests::build_hardened_projection_summary ... ok
test tests::build_memory_projection_summary ... ok
test tests::build_reconcile_projection_summary ... ok
test tests::query_records_by_kind_and_case ... ok
test tests::parse_jsonl_and_build_projection_summary ... ok
test tests::rust_engine_ffi_append_and_count ... ok

test result: ok. 11 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

     Running unittests src/lib.rs (target/debug/deps/yai_core_engine_sys-24677f9bc7566579)

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

   Doc-tests yai_core_engine

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

   Doc-tests yai_core_engine_sys

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

CARGO_TARGET_DIR=target cargo build --manifest-path cmd/yai/Cargo.toml
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.01s
CARGO_TARGET_DIR=target cargo test --manifest-path cmd/yai/Cargo.toml
    Finished `test` profile [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/main.rs (target/debug/deps/yai-7985b15b2fa747b6)

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

installed: /tmp/yai-install-test/bin/yai
installed: /tmp/yai-install-test/bin/yaid
yai local doctor
PREFIX: /tmp/yai-install-test
YAI_HOME: /tmp/yai-home-test
YAI_HOME_status: ok
binary_path: /tmp/yai-install-test/bin/yai
yaid_path: /tmp/yai-install-test/bin/yaid
yai_version: yai 0.0.0-newcore
run_dir: /tmp/yai-home-test/run
store_dir: /tmp/yai-home-test/store
record_store_path: /tmp/yai-home-test/store/lmdb
record_store_status: not_initialized
record_store_backend: lmdb
log_dir: /tmp/yai-home-test/log
tmp_dir: /tmp/yai-home-test/tmp
cases_dir: /tmp/yai-home-test/cases
sockets_dir: /tmp/yai-home-test/sockets
config_dir: /tmp/yai-home-test/config
daemon_socket_default: /tmp/yai-home-test/run/yaid.sock
hot_state_path: /tmp/yai-home-test/run/hot-state.json
hot_state_status: missing
runtime_layout_status: ok
PATH_status: warning add /tmp/yai-install-test/bin to PATH
yai 0.0.0-newcore
yai: technical YAI control command
status: SPINE.32 LMDB Case / Subject / Receipt Indexes
ownership: Rust client over C-defined core primitives
daemon_ipc: local Unix socket with daemon-backed loop v0
canonical_daemon: yaid
runtime_layout: YAI_HOME local runtime v0
foundation_freeze: filesystem_runtime_layout
hot_state: YAI_HOME/run/hot-state.json live cache v0
record_store: YAI_HOME/store/lmdb planned LMDB lookup plane
journal_inspection: file-based JSONL v0
control_inspection: journal-derived summary
yai doctor: ok
public_semantics: C ABI + core docs
rust_role: yai client and operational data engine skeleton
journal_mode: file debug only
binary_path: /tmp/yai-install-test/bin/yai
yaid_path: /tmp/yai-install-test/bin/yaid
yaid_found: found
yai_version: 0.0.0-newcore
YAI_HOME: /tmp/yai-home-test
YAI_HOME_status: ok
run_dir: /tmp/yai-home-test/run ok
store_dir: /tmp/yai-home-test/store ok
log_dir: /tmp/yai-home-test/log ok
tmp_dir: /tmp/yai-home-test/tmp ok
cases_dir: /tmp/yai-home-test/cases ok
sockets_dir: /tmp/yai-home-test/sockets ok
config_dir: /tmp/yai-home-test/config ok
env_file: not found
PATH_status: current binary dir present
daemon_socket_default: /tmp/yai-home-test/run/yaid.sock
socket_default_status: not_present
hot_state_path: /tmp/yai-home-test/run/hot-state.json
hot_state_status: unavailable
hot_state_schema_status: missing
hot_state_readable: no
record_store_path: /tmp/yai-home-test/store/lmdb
record_store_status: not_initialized
record_store_backend: lmdb
runtime_layout_status: ok
hot_state: unavailable
reason: missing_snapshot
snapshot: /tmp/yai-home-test/run/hot-state.json
snapshot_path: /tmp/yai-home-test/run/hot-state.json
snapshot_status: unavailable
schema: unknown
case_session: unknown
case_world: unknown
case_context: unknown
active_thread: unknown
participant_view: unknown
projection: unknown
projection_policy: unknown
freshness_policy: unknown
stale_reason: unknown
yaid 0.0.0-newcore

```

Exit status: `0`


## Policy pack fixture checkpoint

```sh

set -eu
PACK_SRC='/home/mothx/COMPUTER_SCIENCE/DEV_CODE/YAI/yai/docs/manuals/examples/filesystem-loop/policy-packs'
PACK_DST='/tmp/yai-home-test/cases/manual-filesystem-loop/policy-packs'
mkdir -p "$PACK_DST"
cp "$PACK_SRC"/*.json "$PACK_DST"/
python -m json.tool "$PACK_DST/filesystem-sandbox-policy.json" >/dev/null
python -m json.tool "$PACK_DST/model-case-projection-policy.json" >/dev/null
python -m json.tool "$PACK_DST/linenoise-terminal-interface-policy.json" >/dev/null
ls "$PACK_DST"

```

```text
filesystem-sandbox-policy.json
linenoise-terminal-interface-policy.json
model-case-projection-policy.json

```

Exit status: `0`


## Start daemon

```sh
mkdir -p '/tmp/yai-home-test/run'
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yaid --socket '/tmp/yai-home-test/run/yaid.sock' --foreground > /tmp/yai-command-surface-yaid.log 2>&1 &
```

```text
daemon started: pid=399258 socket=/tmp/yai-home-test/run/yaid.sock log=/tmp/yai-command-surface-yaid.log

```

Exit status: `0`


## Daemon status/info

```sh

set -eu
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai daemon status --socket '/tmp/yai-home-test/run/yaid.sock'
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai daemon info --socket '/tmp/yai-home-test/run/yaid.sock'

```

```text
{"ok":true,"version":"0.0.0-newcore","status":"ok","message":"yaid local ipc ready"}
{"ok":true,"version":"0.0.0-newcore","status":"ok","message":"yaid NEW.12 daemon-backed core loop v0"}

```

Exit status: `0`


## Filesystem loop and hot state

```sh
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai daemon run-minimum-loop --socket '/tmp/yai-home-test/run/yaid.sock'
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai hot status
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai daemon run-filesystem-loop --socket '/tmp/yai-home-test/run/yaid.sock'
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai hot status
```

```text
{"request_id":"yai-minimum","ok":true,"version":"0.0.0-newcore","status":"completed","message":"daemon minimum loop completed","journal_path":"build/tmp/new12/daemon-399258/minimum-loop/journal.jsonl","record_count":8,"receipt_count":1,"projection_count":1,"decision_outcome":"require_review","receipt_status":"blocked","fs_read":"","fs_write_blocked":"","fs_write_allowed":"","case_session":"","case_world":"","case_context":"","authority_scope":""}
hot_state: active
snapshot: /tmp/yai-home-test/run/hot-state.json
snapshot_path: /tmp/yai-home-test/run/hot-state.json
snapshot_status: active
schema: yai.hot_state.v1
case: case:new12-daemon
session: active
case_session: active
case_world: loaded
context: active
case_context: active
case_session_id: session:new12-minimum
case_context_id: case_context:minimum
active_thread: none
participant_view: none
case_version: 8
projection: fresh
projection_policy: usable
freshness_policy: usable
projection_freshness: fresh
stale_reason: none
projection_stale_reason: none
last_record: none
last_decision: decision:new12-minimum
last_receipt: receipt:new12-minimum
pending_ops: 0
pending_obligations: 0
carrier_locks: 0
updated_at: 1779640106000
{"request_id":"yai-filesystem","ok":true,"version":"0.0.0-newcore","status":"completed","message":"daemon filesystem loop completed","journal_path":"build/tmp/new12/daemon-399258/filesystem/journal.jsonl","record_count":28,"receipt_count":3,"projection_count":2,"decision_outcome":"allow_with_constraints","receipt_status":"executed","fs_read":"observed","fs_write_blocked":"blocked","fs_write_allowed":"executed","case_session":"active","case_world":"loaded","case_context":"active","authority_scope":"model:interpretation_only terminal:display_only"}
hot_state: active
snapshot: /tmp/yai-home-test/run/hot-state.json
snapshot_path: /tmp/yai-home-test/run/hot-state.json
snapshot_status: active
schema: yai.hot_state.v1
case: case:new12-filesystem
session: active
case_session: active
case_world: loaded
context: active
case_context: active
case_session_id: session:new12-filesystem
case_context_id: case_context:active
active_thread: none
participant_view: none
case_version: 30
projection: fresh
projection_policy: usable
freshness_policy: usable
projection_freshness: fresh
stale_reason: none
projection_stale_reason: none
last_record: none
last_decision: decision:new12-fs-write
last_receipt: receipt:new12-fs-write
pending_ops: 0
pending_obligations: 0
carrier_locks: 0
updated_at: 1779640106000
JOURNAL=build/tmp/new12/daemon-399258/filesystem/journal.jsonl

```

Exit status: `0`


## Projection freshness inspection

```sh

set -eu
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai projection inspect --journal 'build/tmp/new12/daemon-399258/filesystem/journal.jsonl' --consumer model

```

```text
records: 28
case: case:new12-filesystem
case_domains: 1
case_attachments: 1
case_bindings: 1
interaction_threads: 0
interaction_turns: 0
participant_view_frames: 0
projection_requests: 1
projection_results: 2
projection_rules: 1
authority_scopes: 3
model_interpretations: 0
operator: 1
model: 1
audit: 0
redacted_or_limited: 1
consumer: model
projection_freshness: fresh
stale_reason: none
freshness_policy: usable
freshness_source: hot_state
source: hot_state

```

Exit status: `0`


## Snapshot edge cases

```sh

set -eu
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai daemon shutdown --socket '/tmp/yai-home-test/run/yaid.sock'
rm -f '/tmp/yai-home-test/run/hot-state.json'
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai hot status || true
printf '{broken\n' > '/tmp/yai-home-test/run/hot-state.json'
PATH='/tmp/yai-install-test/bin':"$PATH" YAI_HOME='/tmp/yai-home-test' yai hot status || true

```

```text
{"ok":true,"version":"0.0.0-newcore","status":"ok","message":"yaid shutdown accepted"}
hot_state: unavailable
reason: missing_snapshot
snapshot: /tmp/yai-home-test/run/hot-state.json
snapshot_path: /tmp/yai-home-test/run/hot-state.json
snapshot_status: unavailable
schema: unknown
case_session: unknown
case_world: unknown
case_context: unknown
active_thread: unknown
participant_view: unknown
projection: unknown
projection_policy: unknown
freshness_policy: unknown
stale_reason: unknown
hot_state: unavailable
reason: invalid_snapshot
snapshot: /tmp/yai-home-test/run/hot-state.json
snapshot_path: /tmp/yai-home-test/run/hot-state.json
snapshot_status: unavailable
schema: unknown
case_session: unknown
case_world: unknown
case_context: unknown
active_thread: unknown
participant_view: unknown
projection: unknown
projection_policy: unknown
freshness_policy: unknown
stale_reason: unknown

```

Exit status: `0`


## make smoke-spine24a

```sh
$ make smoke-spine24a
```

```text
CARGO_TARGET_DIR=target cargo build --manifest-path engine/Cargo.toml --workspace
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.00s
CARGO_TARGET_DIR=target cargo test --manifest-path engine/Cargo.toml --workspace
    Finished `test` profile [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/lib.rs (target/debug/deps/yai_core_engine-64d462f22a0113a9)

running 11 tests
test tests::append_record_and_build_projection_summary ... ok
test tests::build_authority_projection_summary ... ok
test tests::build_control_projection_summary ... ok
test tests::build_filesystem_projection_summary ... ok
test tests::build_memory_projection_summary ... ok
test tests::build_hardened_projection_summary ... ok
test tests::build_graph_projection_summary ... ok
test tests::build_reconcile_projection_summary ... ok
test tests::parse_jsonl_and_build_projection_summary ... ok
test tests::query_records_by_kind_and_case ... ok
test tests::rust_engine_ffi_append_and_count ... ok

test result: ok. 11 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

     Running unittests src/lib.rs (target/debug/deps/yai_core_engine_sys-24677f9bc7566579)

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

   Doc-tests yai_core_engine

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

   Doc-tests yai_core_engine_sys

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

CARGO_TARGET_DIR=target cargo build --manifest-path cmd/yai/Cargo.toml
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.00s
CARGO_TARGET_DIR=target cargo test --manifest-path cmd/yai/Cargo.toml
    Finished `test` profile [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/main.rs (target/debug/deps/yai-7985b15b2fa747b6)

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

command_surface:install-paths ok
command_surface:doctrine-checks ok
command_surface:runtime-info ok
command_surface:hot-state missing ok
command_surface:hot-state corrupt ok
command_surface:daemon-status ok
command_surface:hot-state valid ok
command_surface:spine24a ok

```

Exit status: `0`


## make smoke-spine27

```sh
$ make smoke-spine27
```

```text
CARGO_TARGET_DIR=target cargo build --manifest-path engine/Cargo.toml --workspace
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.00s
CARGO_TARGET_DIR=target cargo test --manifest-path engine/Cargo.toml --workspace
    Finished `test` profile [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/lib.rs (target/debug/deps/yai_core_engine-64d462f22a0113a9)

running 11 tests
test tests::append_record_and_build_projection_summary ... ok
test tests::build_control_projection_summary ... ok
test tests::build_authority_projection_summary ... ok
test tests::build_filesystem_projection_summary ... ok
test tests::build_graph_projection_summary ... ok
test tests::build_memory_projection_summary ... ok
test tests::build_reconcile_projection_summary ... ok
test tests::build_hardened_projection_summary ... ok
test tests::query_records_by_kind_and_case ... ok
test tests::parse_jsonl_and_build_projection_summary ... ok
test tests::rust_engine_ffi_append_and_count ... ok

test result: ok. 11 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

     Running unittests src/lib.rs (target/debug/deps/yai_core_engine_sys-24677f9bc7566579)

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

   Doc-tests yai_core_engine

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

   Doc-tests yai_core_engine_sys

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

CARGO_TARGET_DIR=target cargo build --manifest-path cmd/yai/Cargo.toml
    Finished `dev` profile [unoptimized + debuginfo] target(s) in 0.00s
CARGO_TARGET_DIR=target cargo test --manifest-path cmd/yai/Cargo.toml
    Finished `test` profile [unoptimized + debuginfo] target(s) in 0.00s
     Running unittests src/main.rs (target/debug/deps/yai-7985b15b2fa747b6)

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

hot_cli:missing snapshot ok
hot_cli:corrupt snapshot ok
hot_cli:minimum hot status ok
hot_cli:filesystem hot status ok
hot_cli:projection freshness inspect ok
hot_cli:doctor hot state ok
hot_cli:spine27 ok

```

Exit status: `0`


## Uninstall local runtime

```sh

set -eu
make uninstall-local PREFIX='/tmp/yai-install-test'
test ! -e '/tmp/yai-install-test/bin/yai'
test ! -e '/tmp/yai-install-test/bin/yaid'

```

```text
uninstalled local yai binaries from /tmp/yai-install-test/bin

```

Exit status: `0`


## Summary

- Report saved at `/home/mothx/COMPUTER_SCIENCE/DEV_CODE/YAI/yai/docs/manuals/outputs/filesystem-loop-validation-2026-05-24.md`.
- Filesystem journal observed at `build/tmp/new12/daemon-399258/filesystem/journal.jsonl`.
- LAN provider prompt cells were not run.
