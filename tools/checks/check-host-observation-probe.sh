#!/usr/bin/env bash
# YAI - host observation probe doctrine check
#
# Purpose:
#   Prevent host observation from being confused with enforcement or repair.
#
# Scope:
#   Checks SPINE.33E docs and headers for observation/mismatch doctrine.
#
# Non-goals:
#   Does not probe real host processes.

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

require_file docs/architecture/22-host-observation-probe.md
require_file docs/engineering/host-observation-probe.md
require_file docs/adr/0024-host-observation-probe.md
require_file docs/archive/engineering/waves/spine33e-host-observation-probe.md
require_file include/yai/observation/host_probe.h
require_file include/yai/observation/observation_result.h
require_file include/yai/observation/observation_target.h
require_file include/yai/reconcile/divergence_candidate.h

doctrine="docs/architecture/22-host-observation-probe.md"
commands="docs/engineering/command-surface.md"

for text in \
  "observation is not enforcement" \
  "host probe" \
  "expected state" \
  "observed state" \
  "mismatch" \
  "divergence_candidate" \
  "process observer" \
  "kill(pid, 0)" \
  "POSIX probe" \
  "no silent repair"
do
  require_text "$doctrine" "$text"
done

require_text "$commands" "yai observe process --pid"
require_text "$commands" "yai observe compare-process --pid"

printf 'host-observation-probe: ok\n'
