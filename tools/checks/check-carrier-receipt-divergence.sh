#!/usr/bin/env bash
set -euo pipefail

# YAI - carrier receipt divergence doctrine check
#
# Purpose:
#   Guard SPINE.33I carrier receipt / divergence hardening surfaces.
#
# Boundary:
#   This script validates source/docs presence only. It does not execute
#   carriers or repair divergence.

require_file() {
  test -f "$1" || {
    printf 'missing required file: %s\n' "$1" >&2
    exit 1
  }
}

require_text() {
  local file="$1"
  local text="$2"
  grep -F -- "$text" "$file" >/dev/null || {
    printf 'missing required text in %s: %s\n' "$file" "$text" >&2
    exit 1
  }
}

require_file include/yai/reconcile/carrier_consistency.h
require_file system/reconcile/carrier_consistency.c
require_file work/archive/architecture-snapshots/30-carrier-receipt-divergence.md
require_file work/archive/engineering-snapshots/carrier-receipt-divergence.md
require_file work/archive/adr/0030-carrier-receipt-divergence.md
require_file work/archive/legacy-docs/engineering/waves/spine33i-carrier-receipt-divergence.md
require_file tests/smoke/carrier-receipt-divergence/test_carrier_receipt_divergence.c

for text in \
  "decision" \
  "dispatch" \
  "carrier outcome" \
  "receipt posture" \
  "observation result" \
  "divergence candidate" \
  "denied_but_attempted" \
  "executed_without_receipt" \
  "blocked_but_effect_observed" \
  "receipt_claimed_executed_but_not_observed" \
  "failed_with_partial_effect" \
  "skeleton_executed_unexpectedly" \
  "no silent repair" \
  "yai carrier reconcile-outcome"
do
  require_text work/archive/engineering-snapshots/carrier-receipt-divergence.md "$text"
done

require_text cmd/yai/src/main.rs "reconcile-outcome"
require_text Makefile "smoke-spine33i"
require_text Makefile "check-carrier-receipt-divergence"

printf "carrier-receipt-divergence: ok\n"
