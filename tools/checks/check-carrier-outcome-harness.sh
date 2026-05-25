#!/usr/bin/env bash
# YAI - carrier outcome harness doctrine check
#
# Purpose:
#   Ensure SPINE.33H outcome harness files, docs and command surface exist.
#
# Scope:
#   Checks guardable static text and required source files.
#
# Non-goals:
#   Does not execute real carrier effects.

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

require_file include/yai/effect/carrier_outcome_harness.h
require_file system/effect/carrier_outcome_harness.c
require_file tests/smoke/carrier-outcome-harness/test_carrier_outcome_harness.c
require_file docs/architecture/25-carrier-outcome-harness.md
require_file docs/engineering/carrier-outcome-harness.md
require_file docs/adr/0027-carrier-outcome-harness.md
require_file docs/archive/engineering/waves/spine33h-carrier-outcome-harness.md

for text in \
  "carrier outcome harness" \
  "executed" \
  "blocked" \
  "deferred" \
  "failed" \
  "mismatch" \
  "observed" \
  "quarantined" \
  "waiting_operator" \
  "waiting_agent" \
  "not_attempted" \
  "skeleton carriers must never execute" \
  "execution_performed: false" \
  "carrier_attempted: false" \
  "receipt_posture: simulated" \
  "receipt_posture: none" \
  "divergence_candidate" \
  "unsupported_carrier_family" \
  "yai carrier outcome-test"
do
  require_text docs/engineering/carrier-outcome-harness.md "$text"
done

require_text cmd/yai/src/main.rs "outcome-test"
require_text include/yai/yai.h "carrier_outcome_harness.h"
require_text Makefile "check-carrier-outcome-harness"
require_text Makefile "smoke-spine33h"

printf 'carrier-outcome-harness: ok\n'
