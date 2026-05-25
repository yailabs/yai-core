# Filesystem Loop Lab Runbook

Status: canonical lab runbook.

Purpose: run the filesystem-loop validation path without making
`docs/manuals/` a second source of truth.

## Layout

```text
Terminal 1: optional LAN provider host
Terminal 2: yaid daemon on the operator host
Terminal 3: setup, case, attach, prompt and inspection commands
```

## Setup

```bash
set -eu
export PREFIX=/tmp/yai-install-test
export YAI_HOME=/tmp/yai-home-test
export PATH="$PREFIX/bin:$PATH"
export YAI_SOCKET="$YAI_HOME/run/yaid.sock"

rm -rf "$PREFIX" "$YAI_HOME"
mkdir -p "$YAI_HOME/run"
```

## Policy-Pack Fixtures

The filesystem-loop lab uses versioned fixture material. There is no separate
`yai pack install` command in the current core.

```bash
PACK_SRC="$PWD/docs/labs/filesystem-loop/policy-packs"
PACK_DST="$YAI_HOME/cases/manual-filesystem-loop/policy-packs"

mkdir -p "$PACK_DST"
cp "$PACK_SRC"/*.json "$PACK_DST"/

python -m json.tool "$PACK_DST/filesystem-sandbox-policy.json" >/dev/null
python -m json.tool "$PACK_DST/model-case-projection-policy.json" >/dev/null
python -m json.tool "$PACK_DST/linenoise-terminal-interface-policy.json" >/dev/null
```

## Command-Surface Baseline

```bash
make build

target/debug/yai --version
target/debug/yai info
target/debug/yai doctor
target/debug/yai hot status
build/yaid --version

target/debug/yai carrier families
target/debug/yai carrier lanes
target/debug/yai carrier route --family filesystem
target/debug/yai carrier route --family process
target/debug/yai carrier route --family unknown
```

Expected posture:

```text
hot_state_status: unavailable
reason: missing_snapshot
carrier family model_provider is visible
dispatch planning is not execution
execution_performed: false
```

## Daemon Loop

```bash
make install-local PREFIX="$PREFIX" YAI_HOME="$YAI_HOME"

yaid --socket "$YAI_SOCKET" --foreground > "$YAI_HOME/run/yaid.log" 2>&1 &
DAEMON_PID=$!

sleep 1

yai daemon status --socket "$YAI_SOCKET"
yai daemon info --socket "$YAI_SOCKET"
yai daemon run-minimum-loop --socket "$YAI_SOCKET"
yai hot status
yai daemon run-filesystem-loop --socket "$YAI_SOCKET"
yai hot status
```

## Journal Replay to LMDB

SPINE.36 uses the filesystem-loop journal as replay/audit input and materializes
valid records into the LMDB lookup plane.

```bash
JOURNAL="$(find "$YAI_HOME" /tmp -name journal.jsonl 2>/dev/null | tail -n 1)"

yai journal inspect --path "$JOURNAL"
yai journal replay --path "$JOURNAL" --dry-run
yai journal replay --path "$JOURNAL"
yai store summary
yai store record list --case case:new12-filesystem --limit 10
```

Expected replay posture:

```text
replay_ready: yes
journal_replay: dry_run
journal_replay: completed
record_store_status: ready
```

If the journal was already mirrored by the daemon loop, real replay may report
`records_written: 0` and `records_duplicate: N`. That is the expected
idempotent replay posture.

## Replay Idempotency + Schema Version Handling

SPINE.37 adds replay status, cursor and schema compatibility output.

```bash
yai journal replay --path "$JOURNAL" --dry-run
yai journal replay --path "$JOURNAL"
yai journal replay-status --path "$JOURNAL"
yai journal replay --path "$JOURNAL"
```

Expected posture:

```text
journal_identity:
record_schema: yai.record.v1
journal_schema: yai.store.record.v0
compatibility: compatible
cursor_line:
replay_status: completed
idempotent: yes
```

Second replay must not duplicate records. Schema mismatch must not write
records.

## Optional LAN Provider

Use the provider values from `.yai/env` or the NVIDIA LAN benchmark lab. The
current observed LAN provider port is `43117`.

```bash
curl "http://$YAI_PROVIDER_HOST:$YAI_PROVIDER_PORT/v1/models"
yai case attach-provider \
  --journal "$JOURNAL" \
  --case "$YAI_CASE_REF" \
  --subject "$YAI_SUBJECT_REF" \
  --base-url "$YAI_PROVIDER_BASE_URL" \
  --model "$YAI_PROVIDER_MODEL" \
  --api-key-env "${YAI_PROVIDER_API_KEY_ENV:-OPENCODE_LLM_API_KEY}"
```

Provider output remains non-authoritative claim/proposal material. Any proposed
action must become an operation attempt and pass through control/carrier.

## Cleanup

```bash
kill "$DAEMON_PID" 2>/dev/null || true
make uninstall-local PREFIX="$PREFIX" YAI_HOME="$YAI_HOME"
```
## Replay Diagnostics / Rebuild Report

Replay reports are durable operational evidence for rebuild/replay behavior.
They are not conformance fixtures yet, but they prepare future conformance.

```bash
yai journal replay --path <journal.jsonl>
yai journal replay-report --path <journal.jsonl>
```

The report uses `yai.replay_report.v1` and is stored under
`YAI_HOME/store/replay/reports/`. It records `journal_identity`,
`compatibility`, `cursor_line`, `records_written`, `records_duplicate`,
`invalid_entries` and failed report diagnostics for the filesystem-loop journal.
