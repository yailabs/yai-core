#!/usr/bin/env bash
# YAI - carrier contract v1 doctrine check
#
# Purpose:
#   Keep carrier.v1 as controlled effect realization, not free tool execution.
#
# Scope:
#   Checks SPINE.33C docs and headers for contract/receipt boundary terms.
#
# Non-goals:
#   Does not validate carrier runtime behavior or process/network execution.

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

require_file work/archive/architecture-snapshots/20-carrier-contract-v1.md
require_file work/archive/engineering-snapshots/carrier-contract-v1.md
require_file work/archive/adr/0022-carrier-contract-v1.md
require_file work/archive/legacy-docs/engineering/waves/spine33c-carrier-contract-v1-filesystem-adapter.md
require_file include/yai/effect/carrier_contract.h
require_file include/yai/effect/carrier_outcome.h
require_file include/yai/effect/carrier_receipt.h

doctrine="work/archive/architecture-snapshots/20-carrier-contract-v1.md"
engineering="work/archive/engineering-snapshots/carrier-contract-v1.md"
commands="work/spines/command-surface.md"

for text in \
  "carrier.v1" \
  "pre_state_observation" \
  "controlled_execution_or_observation" \
  "post_state_observation" \
  "evidence_capture" \
  "receipt_assembly" \
  "receipt_validation" \
  "residue_recording" \
  "filesystem carrier" \
  "receipt_required" \
  "guarantee_mode" \
  "carrier outcome is not gate outcome"
do
  require_text "$doctrine" "$text"
done

require_text "$engineering" "filesystem is the first carrier.v1 adapter"
require_text "$commands" "yai carrier inspect filesystem"

printf 'carrier-contract-v1: ok\n'
