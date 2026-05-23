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
filesystem_case_entry=$(target/debug/yai case enter --journal "$filesystem_journal" --case case:new12-filesystem --subject subject:llm-provider)
filesystem_case_shell=$(target/debug/yai case enter --journal "$filesystem_journal" --case case:new12-filesystem --subject subject:llm-provider --shell zsh)
filesystem_provider_attach=$(target/debug/yai case attach-provider --journal "$filesystem_journal" --case case:new12-filesystem --subject subject:llm-provider --base-url http://127.0.0.1:43117/v1/chat/completions --model qwen-local)
filesystem_provider_shell=$(target/debug/yai case attach-provider --journal "$filesystem_journal" --case case:new12-filesystem --subject subject:llm-provider --base-url http://127.0.0.1:43117/v1/chat/completions --model qwen-local --shell zsh)
filesystem_transcript_on=$(YAI_JOURNAL="$filesystem_journal" YAI_CASE_REF=case:new12-filesystem YAI_SUBJECT_REF=subject:llm-provider YAI_PROVIDER_BASE_URL=http://127.0.0.1:43117/v1/chat/completions YAI_PROVIDER_MODEL=qwen-local target/debug/yai prompt --once "/transcript on")
filesystem_transcript_status=$(YAI_JOURNAL="$filesystem_journal" YAI_CASE_REF=case:new12-filesystem YAI_SUBJECT_REF=subject:llm-provider YAI_PROVIDER_BASE_URL=http://127.0.0.1:43117/v1/chat/completions YAI_PROVIDER_MODEL=qwen-local target/debug/yai prompt --once "/transcript status")
filesystem_memory_propose=$(YAI_JOURNAL="$filesystem_journal" YAI_CASE_REF=case:new12-filesystem YAI_SUBJECT_REF=subject:llm-provider YAI_PROVIDER_BASE_URL=http://127.0.0.1:43117/v1/chat/completions YAI_PROVIDER_MODEL=qwen-local target/debug/yai prompt --once "/memory propose smoke boundary residue")
filesystem_prompt_dry_run=$(YAI_JOURNAL="$filesystem_journal" YAI_CASE_REF=case:new12-filesystem YAI_SUBJECT_REF=subject:llm-provider YAI_PROVIDER_BASE_URL=http://127.0.0.1:43117/v1/chat/completions YAI_PROVIDER_MODEL=qwen-local target/debug/yai prompt --dry-run --once "What subjects are bound to this case?")
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
printf '%s\n' "$filesystem_case_entry" | grep 'case_entry: accepted' >/dev/null
printf '%s\n' "$filesystem_case_entry" | grep 'subject_ref: subject:llm-provider' >/dev/null
printf '%s\n' "$filesystem_case_entry" | grep 'participant_view: model_context' >/dev/null
printf '%s\n' "$filesystem_case_entry" | grep 'raw_journal_access: not_provided' >/dev/null
printf '%s\n' "$filesystem_case_shell" | grep 'export YAI_CASE_REF=' >/dev/null
printf '%s\n' "$filesystem_case_shell" | grep 'export YAI_CASE_PROMPT_FLAG=' >/dev/null
printf '%s\n' "$filesystem_case_shell" | grep 'export PROMPT=' >/dev/null
printf '%s\n' "$filesystem_case_shell" | grep 'export RPROMPT="$YAI_RPROMPT_BASE"' >/dev/null
printf '%s\n' "$filesystem_provider_attach" | grep 'provider_attachment: accepted' >/dev/null
printf '%s\n' "$filesystem_provider_attach" | grep 'provider_attachment_status: attached' >/dev/null
printf '%s\n' "$filesystem_provider_shell" | grep 'export YAI_PROVIDER_BASE_URL=' >/dev/null
printf '%s\n' "$filesystem_provider_shell" | grep 'export YAI_PROVIDER_MODEL=' >/dev/null
printf '%s\n' "$filesystem_transcript_on" | grep 'prompt_transcript_retention: enabled' >/dev/null
printf '%s\n' "$filesystem_transcript_on" | grep 'transcript_retention: full_redacted_case_local' >/dev/null
printf '%s\n' "$filesystem_transcript_status" | grep 'prompt_transcript_retention: enabled' >/dev/null
printf '%s\n' "$filesystem_transcript_status" | grep 'memory_candidate: derived_not_raw_chat' >/dev/null
printf '%s\n' "$filesystem_memory_propose" | grep 'memory_proposal: accepted' >/dev/null
printf '%s\n' "$filesystem_memory_propose" | grep 'record_kind: memory_candidate' >/dev/null
printf '%s\n' "$filesystem_prompt_dry_run" | grep 'model_prompt: dry_run' >/dev/null
printf '%s\n' "$filesystem_prompt_dry_run" | grep 'context_source: session_participant_view' >/dev/null
printf '%s\n' "$filesystem_prompt_dry_run" | grep 'transcript_retention: full_redacted_case_local' >/dev/null
printf '%s\n' "$filesystem_prompt_dry_run" | grep 'raw_journal_access: not_provided' >/dev/null
printf '%s\n' "$filesystem_prompt_dry_run" | grep 'decision_authority: not_provided' >/dev/null
printf '%s\n' "$filesystem_prompt_dry_run" | grep 'receipt_authority: not_provided' >/dev/null

wait "$daemon_pid"
trap - EXIT INT TERM

printf 'daemon:started\n'
printf 'ipc:status ok\n'
printf 'daemon-loop:minimum completed\n'
printf 'daemon-loop:filesystem completed\n'
printf 'journal:summary ok\n'
printf 'projection:summary ok\n'
printf 'case:enter ok\n'
printf 'case:attach-provider ok\n'
printf 'case:transcript retention ok\n'
printf 'case:memory proposal ok\n'
printf 'case:prompt dry-run ok\n'
printf 'daemon:shutdown ok\n'
