# Manual Command Surface Validation

This is the single operator-facing command-surface runbook for the current
`yai-core` implementation.

Engineering command inventory lives in `docs/engineering/command-surface.md`.
This manual is the place to run commands by hand and preserve evidence. When a
wave adds or changes a command, update this manual and the companion notebook.

Aligned through:

```text
SPINE.27 Hot State CLI + Manual Validation
```

## Scope

This manual validates:

```text
runtime layout
installed yai / yaid binaries
yai doctor
yai hot status
daemon status/info/minimum/filesystem loops
projection inspect freshness output
missing/corrupt hot-state snapshots
uninstall-local
```

It does not validate LMDB, Ladybug, DuckDB, projection deltas, model carrier or
agent harness work.

## Setup

Run from the `yai-core` repository root.

```bash
set -eu
export PREFIX=/tmp/yai-core-install-test
export YAI_HOME=/tmp/yai-core-home-test
export PATH="$PREFIX/bin:$PATH"
export YAI_SOCKET="$YAI_HOME/run/yaid.sock"

rm -rf "$PREFIX" "$YAI_HOME"
```

## Dev Binary Checks

```bash
make build

target/debug/yai --version
target/debug/yai info
target/debug/yai doctor
target/debug/yai hot status
build/yaid --version
```

Expected key lines before daemon activity:

```text
status: SPINE.27 Hot State CLI + Manual Validation
hot_state_path:
hot_state_status: unavailable
hot_state_schema_status: missing
hot_state_readable: no
hot_state: unavailable
reason: missing_snapshot
```

## Installed Runtime Checks

```bash
make install-local PREFIX="$PREFIX" YAI_HOME="$YAI_HOME"

yai --version
yai info
yai doctor
yai hot status
yaid --version
```

Expected key lines:

```text
YAI_HOME: /tmp/yai-core-home-test
runtime_layout_status: ok
hot_state: unavailable
reason: missing_snapshot
```

## Daemon And Hot-State Lifecycle

Start `yaid`:

```bash
mkdir -p "$YAI_HOME/run"

yaid --socket "$YAI_SOCKET" --foreground > /tmp/yai-core-command-surface-yaid.log 2>&1 &
DAEMON_PID=$!

sleep 1
```

Run the daemon loop checks:

```bash
yai daemon status --socket "$YAI_SOCKET"
yai daemon info --socket "$YAI_SOCKET"

yai daemon run-minimum-loop --socket "$YAI_SOCKET"
yai hot status

FILESYSTEM_OUTPUT="$(yai daemon run-filesystem-loop --socket "$YAI_SOCKET")"
printf '%s\n' "$FILESYSTEM_OUTPUT"
yai hot status
```

Expected hot-state lines after the loops:

```text
hot_state: active
schema: yai.hot_state.v1
case_session: active
case_world: loaded
case_context: active
projection: fresh
freshness_policy: usable
stale_reason: none
last_decision:
last_receipt:
```

## Projection Freshness Inspection

Extract the filesystem journal from the daemon response:

```bash
JOURNAL="$(printf '%s\n' "$FILESYSTEM_OUTPUT" | sed -n 's/.*"journal_path":"\([^"]*\)".*/\1/p')"
test -f "$JOURNAL"

yai projection inspect --journal "$JOURNAL" --consumer model
```

Expected key lines:

```text
consumer: model
projection_freshness: fresh
stale_reason: none
freshness_policy: usable
freshness_source: hot_state
```

If `projection inspect` cannot correlate the journal case with active hot
state, it reports projection-record freshness instead. Do not treat that as a
hot-state source.

## Snapshot Edge Cases

Stop the daemon before mutating the snapshot by hand:

```bash
yai daemon shutdown --socket "$YAI_SOCKET"
wait "$DAEMON_PID"
```

Missing snapshot:

```bash
rm -f "$YAI_HOME/run/hot-state.json"
yai hot status
```

Expected:

```text
hot_state: unavailable
reason: missing_snapshot
freshness_policy: unknown
```

Corrupt snapshot:

```bash
printf '{broken\n' > "$YAI_HOME/run/hot-state.json"
yai hot status
```

Expected:

```text
hot_state: unavailable
reason: invalid_snapshot
freshness_policy: unknown
```

## Automated Smoke

```bash
make smoke-spine24a
make smoke-spine27
make smoke
make check
```

## Uninstall

```bash
make uninstall-local PREFIX="$PREFIX"

test ! -e "$PREFIX/bin/yai"
test ! -e "$PREFIX/bin/yaid"
```

`uninstall-local` removes installed binaries only. It does not delete
`YAI_HOME`.

