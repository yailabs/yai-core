#!/bin/sh
set -eu

run_dir="build/tmp/new12/daemon-core-loop-$$"
socket_path="$run_dir/yaid.sock"
log_path="$run_dir/yaid.log"

mkdir -p "$run_dir"
rm -f "$socket_path"

build/yaid --socket "$socket_path" --foreground >"$log_path" 2>&1 &
daemon_pid=$!

cleanup() {
  if kill -0 "$daemon_pid" 2>/dev/null; then
    kill "$daemon_pid" 2>/dev/null || true
    wait "$daemon_pid" 2>/dev/null || true
  fi
}
trap cleanup EXIT INT TERM

tries=0
while [ ! -S "$socket_path" ]; do
  tries=$((tries + 1))
  if [ "$tries" -gt 50 ]; then
    cat "$log_path" >&2 || true
    exit 1
  fi
  sleep 0.1
done

status_output=$(target/debug/yai daemon status --socket "$socket_path")
minimum_output=$(target/debug/yai daemon run-minimum-loop --socket "$socket_path")
minimum_journal=$(printf '%s\n' "$minimum_output" | sed -n 's/.*"journal_path":"\([^"]*\)".*/\1/p')

if [ -z "$minimum_journal" ]; then
  printf '%s\n' "$minimum_output" >&2
  exit 1
fi

journal_output=$(target/debug/yai daemon journal-summary --socket "$socket_path" --journal "$minimum_journal")
projection_output=$(target/debug/yai daemon projection-summary --socket "$socket_path" --journal "$minimum_journal")
filesystem_output=$(target/debug/yai daemon run-filesystem-loop --socket "$socket_path")
filesystem_output=$(target/debug/yai daemon run-filesystem-loop --socket "$socket_path")
filesystem_journal=$(printf '%s\n' "$filesystem_output" | sed -n 's/.*"journal_path":"\([^"]*\)".*/\1/p')

if [ -z "$filesystem_journal" ]; then
  printf '%s\n' "$filesystem_output" >&2
  exit 1
fi

filesystem_summary=$(target/debug/yai daemon journal-summary --socket "$socket_path" --journal "$filesystem_journal")
filesystem_projection=$(target/debug/yai projection inspect --journal "$filesystem_journal")
filesystem_model_context=$(target/debug/yai projection model-context --journal "$filesystem_journal" --case case:new12-filesystem)
filesystem_case_entry=$(target/debug/yai case enter --journal "$filesystem_journal" --case case:new12-filesystem --subject subject:llm-provider)
filesystem_case_shell=$(target/debug/yai case enter --journal "$filesystem_journal" --case case:new12-filesystem --subject subject:llm-provider --shell zsh)
shutdown_output=$(target/debug/yai daemon shutdown --socket "$socket_path")

printf '%s\n' "$status_output" | grep '"status":"ok"' >/dev/null
printf '%s\n' "$minimum_output" | grep '"status":"completed"' >/dev/null
printf '%s\n' "$minimum_output" | grep '"receipt_status":"blocked"' >/dev/null
printf '%s\n' "$journal_output" | grep '"status":"ok"' >/dev/null
printf '%s\n' "$projection_output" | grep '"status":"ok"' >/dev/null
printf '%s\n' "$filesystem_output" | grep '"status":"completed"' >/dev/null
printf '%s\n' "$filesystem_output" | grep '"record_count":23' >/dev/null
printf '%s\n' "$filesystem_output" | grep '"fs_write_blocked":"blocked"' >/dev/null
printf '%s\n' "$filesystem_output" | grep '"fs_write_allowed":"executed"' >/dev/null
printf '%s\n' "$filesystem_summary" | grep '"status":"ok"' >/dev/null
printf '%s\n' "$shutdown_output" | grep '"status":"ok"' >/dev/null
printf '%s\n' "$filesystem_projection" | grep 'projection_results: 2' >/dev/null
printf '%s\n' "$filesystem_projection" | grep 'operator: 1' >/dev/null
printf '%s\n' "$filesystem_projection" | grep 'model: 1' >/dev/null
printf '%s\n' "$filesystem_projection" | grep 'redacted_or_limited: 1' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'consumer: model' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'projection_kind: model_context' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'raw_journal_access: not_provided' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'filesystem_access: not_provided' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'subject:llm-provider' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'subject:linenoise-terminal' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'policy:manual-model-case-projection-v0' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'decision:require_review' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'decision:allow_with_constraints' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'kind:filesystem_receipt' >/dev/null
printf '%s\n' "$filesystem_model_context" | grep 'memory:operational' >/dev/null
printf '%s\n' "$filesystem_case_entry" | grep 'case_entry: accepted' >/dev/null
printf '%s\n' "$filesystem_case_entry" | grep 'subject_ref: subject:llm-provider' >/dev/null
printf '%s\n' "$filesystem_case_entry" | grep 'participant_view: model_context' >/dev/null
printf '%s\n' "$filesystem_case_entry" | grep 'raw_journal_access: not_provided' >/dev/null
printf '%s\n' "$filesystem_case_shell" | grep 'export YAI_CASE_REF=' >/dev/null
printf '%s\n' "$filesystem_case_shell" | grep 'export YAI_CASE_PROMPT_FLAG=' >/dev/null
printf '%s\n' "$filesystem_case_shell" | grep 'export PROMPT=' >/dev/null
printf '%s\n' "$filesystem_case_shell" | grep 'export RPROMPT="$YAI_RPROMPT_BASE"' >/dev/null

wait "$daemon_pid"
trap - EXIT INT TERM

printf 'daemon:started\n'
printf 'ipc:status ok\n'
printf 'daemon-loop:minimum completed\n'
printf 'daemon-loop:filesystem completed\n'
printf 'journal:summary ok\n'
printf 'projection:summary ok\n'
printf 'projection:model-context ok\n'
printf 'case:enter ok\n'
printf 'daemon:shutdown ok\n'
