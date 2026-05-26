#!/usr/bin/env bash
# YAI - process carrier signal control doctrine check
#
# Purpose:
#   Prevent process control from becoming arbitrary host killing.
#
# Scope:
#   Checks SPINE.33D docs and headers for process carrier safety doctrine.
#
# Non-goals:
#   Does not send signals or validate runtime process behavior.

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

require_file work/archive/architecture-snapshots/21-process-carrier-signal-control.md
require_file work/archive/engineering-snapshots/process-carrier-signal-control.md
require_file work/archive/adr/0023-process-carrier-signal-control.md
require_file work/archive/legacy-docs/engineering/waves/spine33d-process-carrier-signal-control.md
require_file include/yai/effect/process_carrier.h
require_file include/yai/effect/process_signal.h
require_file include/yai/effect/process_state.h

doctrine="work/archive/architecture-snapshots/21-process-carrier-signal-control.md"
commands="work/spines/command-surface.md"

for text in \
  "SIGTERM" \
  "SIGKILL" \
  "test-owned process" \
  "carrier_attempted: false" \
  "unsafe_process_target" \
  "process carrier" \
  "carrier.v1" \
  "pre-state observation" \
  "post-state observation" \
  "receipt" \
  "no arbitrary kill"
do
  require_text "$doctrine" "$text"
done

require_text "$commands" "yai process observe --pid"
require_text "$commands" "yai process signal --pid"

printf 'process-carrier-signal-control: ok\n'
