#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
YAI_BIN="$ROOT/target/debug/yai"
YAID="$ROOT/build/yaid"
TMP_DIR="${TMPDIR:-/tmp}/yai-spine51b-$$"
YAI_HOME="$TMP_DIR/home"
SOCKET="$TMP_DIR/run/yaid.sock"
DAEMON_PID=""

cleanup() {
  if [[ -n "$DAEMON_PID" ]]; then
    YAI_HOME="$YAI_HOME" "$YAI_BIN" daemon shutdown --socket "$SOCKET" >/dev/null 2>&1 || true
    wait "$DAEMON_PID" >/dev/null 2>&1 || true
  fi
  rm -rf "$TMP_DIR"
}
trap cleanup EXIT

require_line() {
  local haystack="$1"
  local needle="$2"
  grep -Fq -- "$needle" <<<"$haystack"
}

mkdir -p "$TMP_DIR/run" "$YAI_HOME"
YAI_HOME="$YAI_HOME" "$YAID" --socket "$SOCKET" --foreground &
DAEMON_PID=$!

for _ in $(seq 1 50); do
  [[ -S "$SOCKET" ]] && break
  sleep 0.1
done
[[ -S "$SOCKET" ]]

filesystem_output="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" daemon run-filesystem-loop --socket "$SOCKET")"
journal_path="$(sed -n 's/.*"journal_path":"\([^"]*\)".*/\1/p' <<<"$filesystem_output")"
if [[ -z "$journal_path" ]]; then
  printf '%s\n' "$filesystem_output" >&2
  exit 1
fi

YAI_HOME="$YAI_HOME" "$YAI_BIN" journal replay --path "$journal_path" >/dev/null
YAI_HOME="$YAI_HOME" "$YAI_BIN" daemon run-filesystem-review-loop --socket "$SOCKET" >/dev/null

resolve_model="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" case resolve --case case:new12-filesystem --subject subject:llm-provider)"
require_line "$resolve_model" "case_handle:"
require_line "$resolve_model" "status: resolved"
require_line "$resolve_model" "subject_ref: subject:llm-provider"
require_line "$resolve_model" "refs_are_authority: false"
printf "case_handle:resolve llm-provider ok\n"

resolve_fs="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" case resolve --case case:new12-filesystem --subject subject:filesystem-sandbox)"
require_line "$resolve_fs" "case_handle:"
require_line "$resolve_fs" "status: resolved"
require_line "$resolve_fs" "subject_ref: subject:filesystem-sandbox"
printf "case_handle:resolve filesystem-sandbox ok\n"

scope_model="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" case scope --case case:new12-filesystem --subject subject:llm-provider)"
require_line "$scope_model" "can_execute: no"
require_line "$scope_model" "can_approve: no"
require_line "$scope_model" "raw_journal_access: not_provided"
require_line "$scope_model" "refs_are_authority: false"
printf "case_scope:model no_execute ok\n"

scope_reviewer="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" case scope --case case:new12-filesystem --subject subject:operator-reviewer)"
require_line "$scope_reviewer" "can_review: yes"
require_line "$scope_reviewer" "can_approve: yes"
require_line "$scope_reviewer" "can_deny: yes"
require_line "$scope_reviewer" "can_defer: yes"
require_line "$scope_reviewer" "can_quarantine: yes"
printf "case_scope:reviewer approve ok\n"

model_write="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" capability derive --case case:new12-filesystem --subject subject:llm-provider --operation filesystem.write --resource sandbox/output.txt)"
require_line "$model_write" "lease_status: denied"
require_line "$model_write" "carrier_dispatch_allowed: no"
require_line "$model_write" "reason: subject_lacks_execute_authority"
printf "capability:model_write denied ok\n"

fs_write="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" capability derive --case case:new12-filesystem --subject subject:filesystem-sandbox --operation filesystem.write --resource sandbox/output.txt)"
require_line "$fs_write" "lease_status: requires_review"
require_line "$fs_write" "requires_review: yes"
require_line "$fs_write" "receipt_required: yes"
require_line "$fs_write" "policy_basis: mutative_operation_requires_review"
printf "capability:filesystem_write requires_review ok\n"

fs_read="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" capability derive --case case:new12-filesystem --subject subject:filesystem-sandbox --operation filesystem.read --resource sandbox/input.txt)"
require_line "$fs_read" "lease_status: minted"
require_line "$fs_read" "carrier_dispatch_allowed: yes"
require_line "$fs_read" "receipt_required: yes"
printf "capability:filesystem_read minted ok\n"

outside="$(YAI_HOME="$YAI_HOME" "$YAI_BIN" capability derive --case case:new12-filesystem --subject subject:filesystem-sandbox --operation filesystem.write --resource ../outside.txt)"
require_line "$outside" "lease_status: denied"
require_line "$outside" "resource_scope: outside_sandbox"
require_line "$outside" "reason: resource_outside_scope"
printf "capability:outside_sandbox denied ok\n"

require_line "$outside" "refs_are_authority: false"
printf "refs:not_authority ok\n"
