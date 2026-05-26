#!/usr/bin/env bash
# YAI - provider runtime LAN target surface guard
#
# Purpose:
#   Ensure SPINE.33L provider runtime / LAN target surface remains documented
#   and wired into command, testing and roadmap docs.
#
# Scope:
#   Checks docs and smoke target references only.
#
# Non-goals:
#   Does not validate real provider execution, LAN discovery or model runtime
#   implementation.

set -euo pipefail

require_file() {
  test -f "$1" || {
    printf 'missing required file: %s\n' "$1" >&2
    exit 1
  }
}

require_text() {
  local file="$1"
  local text="$2"
  grep -F "$text" "$file" >/dev/null || {
    printf 'missing required text in %s: %s\n' "$file" "$text" >&2
    exit 1
  }
}

roadmap="work/spines/yai-spine.md"
current="work/spines/current-status.md"
commands="work/spines/command-surface.md"
testing="work/spines/testing.md"
architecture="work/archive/architecture-snapshots/28-provider-runtime-lan-target-surface.md"
engineering="work/archive/engineering-snapshots/provider-runtime-lan-target-surface.md"
adr="work/archive/adr/0028-provider-runtime-lan-target-surface.md"
archive="work/archive/legacy-docs/engineering/waves/spine33l-provider-runtime-lan-target-surface.md"
makefile="Makefile"

for file in "$roadmap" "$current" "$commands" "$testing" "$architecture" "$engineering" "$adr" "$archive" "$makefile"; do
  require_file "$file"
done

for text in \
  "runtime target" \
  "local" \
  "lan" \
  "external" \
  "device registry" \
  "paired device" \
  "trusted device" \
  "provider supervisor" \
  "provider start dry-run" \
  "provider logs" \
  "model catalog" \
  "model routing" \
  "context compiler" \
  "retrieval_hnsw" \
  "decoding acceleration" \
  "no manual provider launch as normal workflow" \
  "no remote start in SPINE.33L"
do
  require_text "$architecture" "$text"
done

for text in \
  "yai device add" \
  "yai device list" \
  "yai device inspect" \
  "yai device trust" \
  "yai provider runtime status" \
  "yai provider targets" \
  "yai provider start --dry-run" \
  "yai provider logs-path" \
  "yai model catalog status" \
  "yai model runtime status" \
  "execution_performed: false"
do
  require_text "$commands" "$text"
done

require_text "$roadmap" "SPINE.33L Provider Runtime / LAN Target Surface v0"
require_text "$current" "SPINE.33L Provider Runtime / LAN Target Surface v0"
require_text "$testing" "SPINE.33L Provider Runtime / LAN Target Surface Loop"
require_text "$engineering" "YAI_HOME/config/devices.jsonl"
require_text "$adr" "YAI must not require manual provider process launch as the normal workflow"
require_text "$archive" "No actual provider process start."
require_text "$makefile" "smoke-spine33l"

printf 'provider-runtime-lan-target-surface: ok\n'
