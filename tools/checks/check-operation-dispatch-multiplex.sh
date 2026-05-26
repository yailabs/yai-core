#!/usr/bin/env bash
# YAI - operation dispatch multiplex doctrine check
#
# Purpose:
#   Prevent dispatch planning from drifting into decision or execution.
#
# Scope:
#   Checks SPINE.33B docs and CLI docs for lane/multiplex doctrine terms.
#
# Non-goals:
#   Does not validate runtime carrier behavior or process/network execution.

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

require_file work/archive/architecture-snapshots/19-operation-dispatch-multiplex.md
require_file work/archive/engineering-snapshots/operation-dispatch-multiplex.md
require_file work/archive/adr/0021-operation-dispatch-multiplex.md
require_file work/archive/legacy-docs/engineering/waves/spine33b-operation-dispatch-multiplex.md
require_file include/yai/effect/dispatch_lane.h
require_file include/yai/effect/dispatch_plan.h
require_file include/yai/effect/dispatch_multiplex.h

doctrine="work/archive/architecture-snapshots/19-operation-dispatch-multiplex.md"
engineering="work/archive/engineering-snapshots/operation-dispatch-multiplex.md"
commands="work/spines/command-surface.md"

for text in \
  "dispatch is separate from decision" \
  "dispatch is separate from execution" \
  "carrier lanes" \
  "no single global queue" \
  "filesystem_lane" \
  "process_lane" \
  "network_http_lane" \
  "database_lane" \
  "repository_git_lane" \
  "model_provider_lane" \
  "observation_lane" \
  "review_lane" \
  "execution_performed: false"
do
  require_text "$doctrine" "$text"
done

require_text "$engineering" "dispatch plan"
require_text "$commands" "yai carrier lanes"
require_text "$commands" "yai carrier route --family"

printf 'operation-dispatch-multiplex: ok\n'
