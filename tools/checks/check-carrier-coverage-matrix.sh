#!/usr/bin/env bash
# YAI - carrier coverage matrix doctrine check
#
# Purpose:
#   Ensure every effect-capable carrier family is visible in the coverage matrix.
#
# Scope:
#   Checks SPINE.33F docs and headers for mode/outcome/family coverage.
#
# Non-goals:
#   Does not execute carriers or validate adapter implementations.

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

require_file work/archive/architecture-snapshots/23-carrier-coverage-matrix.md
require_file work/archive/engineering-snapshots/carrier-coverage-matrix.md
require_file work/archive/adr/0025-carrier-coverage-matrix.md
require_file work/archive/legacy-docs/engineering/waves/spine33f-carrier-coverage-matrix.md
require_file include/yai/effect/carrier_mode.h
require_file include/yai/effect/carrier_coverage.h

doctrine="work/archive/architecture-snapshots/23-carrier-coverage-matrix.md"
commands="work/spines/command-surface.md"

for text in \
  "controlled" \
  "observed" \
  "imported" \
  "executed" \
  "blocked" \
  "failed" \
  "mismatch" \
  "filesystem" \
  "process" \
  "network_http" \
  "database" \
  "repository_git" \
  "service" \
  "endpoint" \
  "socket" \
  "listener" \
  "model_provider" \
  "review" \
  "unknown" \
  "carrier coverage matrix" \
  "no fake execution"
do
  require_text "$doctrine" "$text"
done

require_text "$commands" "yai carrier coverage"
require_text "$commands" "carrier coverage matrix"

printf 'carrier-coverage-matrix: ok\n'
