#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../../.." && pwd)
YAI_HOME=${YAI_HOME:-"/tmp/yai-spine33l-home-$$"}
export YAI_HOME

YAI_BIN="$ROOT/target/debug/yai"

cleanup() {
  rm -rf "$YAI_HOME"
}
trap cleanup EXIT INT TERM

require_line() {
  output="$1"
  expected="$2"
  if ! printf '%s\n' "$output" | grep -Fq -- "$expected"; then
    printf 'missing expected line: %s\noutput:\n%s\n' "$expected" "$output" >&2
    exit 1
  fi
}

reject_line() {
  output="$1"
  rejected="$2"
  if printf '%s\n' "$output" | grep -Fq -- "$rejected"; then
    printf 'unexpected line: %s\noutput:\n%s\n' "$rejected" "$output" >&2
    exit 1
  fi
}

rm -rf "$YAI_HOME"

empty=$("$YAI_BIN" device list)
require_line "$empty" "devices: none"
require_line "$empty" "device_registry: $YAI_HOME/config/devices.jsonl"
printf 'device_registry:empty ok\n'

add=$("$YAI_BIN" device add --id workstation --name Workstation --host 192.168.1.50 --port 8777 --target lan)
require_line "$add" "device:add ok"
require_line "$add" "device_id: workstation"
require_line "$add" "pairing_status: paired"
require_line "$add" "trust_status: untrusted"
printf 'device:add ok\n'

inspect=$("$YAI_BIN" device inspect workstation)
require_line "$inspect" "device_id: workstation"
require_line "$inspect" "target_mode: lan"
require_line "$inspect" "provider_supervisor_status: not_checked"
require_line "$inspect" "model_catalog: not_scanned"
printf 'device:inspect ok\n'

runtime=$("$YAI_BIN" provider runtime status)
require_line "$runtime" "provider_runtime:"
require_line "$runtime" "provider_supervision: planned"
require_line "$runtime" "device_registry: active"
require_line "$runtime" "requires_paired_device: yes"
printf 'provider_runtime:status ok\n'

targets=$("$YAI_BIN" provider targets)
require_line "$targets" "runtime_targets:"
require_line "$targets" "- local"
require_line "$targets" "- lan"
require_line "$targets" "- external"
printf 'provider_runtime:targets ok\n'

local_plan=$("$YAI_BIN" provider start --dry-run --target local --kind llama_server --model qwen3-8b-q4)
require_line "$local_plan" "target: local"
require_line "$local_plan" "provider_kind: llama_server"
require_line "$local_plan" "status: planned"
require_line "$local_plan" "execution_performed: false"
printf 'provider_start:dry_run_local ok\n'

lan_blocked=$("$YAI_BIN" provider start --dry-run --target lan --device workstation --kind ds4 --model deepseek-v4-flash)
require_line "$lan_blocked" "target: lan"
require_line "$lan_blocked" "device_id: workstation"
require_line "$lan_blocked" "status: blocked"
require_line "$lan_blocked" "reason: device_not_trusted"
require_line "$lan_blocked" "execution_performed: false"
printf 'provider_start:dry_run_lan_blocked_untrusted ok\n'

trust=$("$YAI_BIN" device trust workstation)
require_line "$trust" "device_id: workstation"
require_line "$trust" "trust_status: trusted"
printf 'device:trust ok\n'

lan_plan=$("$YAI_BIN" provider start --dry-run --target lan --device workstation --kind ds4 --model deepseek-v4-flash)
require_line "$lan_plan" "target: lan"
require_line "$lan_plan" "device_id: workstation"
require_line "$lan_plan" "provider_kind: ds4"
require_line "$lan_plan" "model: deepseek-v4-flash"
require_line "$lan_plan" "status: planned"
require_line "$lan_plan" "would_contact_device: true"
require_line "$lan_plan" "would_start_provider: true"
require_line "$lan_plan" "execution_performed: false"
printf 'provider_start:dry_run_lan_trusted ok\n'

external_plan=$("$YAI_BIN" provider start --dry-run --target external --endpoint http://127.0.0.1:43117/v1/chat/completions --kind openai_compatible)
require_line "$external_plan" "target: external"
require_line "$external_plan" "provider_kind: openai_compatible"
require_line "$external_plan" "action: attach_only"
require_line "$external_plan" "execution_performed: false"
printf 'provider_start:dry_run_external_attach_only ok\n'

paths=$("$YAI_BIN" provider logs-path)
require_line "$paths" "provider_log_dir: $YAI_HOME/log/providers"
require_line "$paths" "provider_run_dir: $YAI_HOME/run/providers"
require_line "$paths" "provider_plan_dir: $YAI_HOME/tmp/provider-plans"
printf 'provider_logs:path ok\n'

catalog=$("$YAI_BIN" model catalog status)
require_line "$catalog" "model_catalog:"
require_line "$catalog" "current_models: not_scanned"

model_runtime=$("$YAI_BIN" model runtime status)
require_line "$model_runtime" "model_runtime:"
require_line "$model_runtime" "model_routing: planned"
require_line "$model_runtime" "retrieval_hnsw: planned"
require_line "$model_runtime" "decoding_acceleration: planned"
require_line "$model_runtime" "fallback: normal_decoding"
printf 'model_runtime:status ok\n'

info=$("$YAI_BIN" info)
require_line "$info" "status: SPINE.48 Model Behavior / Policy Outcome Facts"
require_line "$info" "provider-runtime: planned surface active"
require_line "$info" "device-registry: active"

reject_line "$local_plan" "started"
reject_line "$lan_plan" "started"
reject_line "$external_plan" "started"
printf 'no_provider_execution:confirmed\n'
