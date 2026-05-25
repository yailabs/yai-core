#!/usr/bin/env bash
# YAI - non-process carrier skeleton doctrine check
#
# Purpose:
#   Ensure SPINE.33G skeleton carriers are documented, inspectable and guarded.
#
# Scope:
#   Checks the static skeleton ABI, docs, CLI source and smoke target.
#
# Non-goals:
#   Does not execute carrier effects or activate adapters.

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

require_file include/yai/effect/carrier_skeleton.h
require_file system/effect/carrier_skeleton.c
require_file tests/smoke/non-process-carrier-skeletons/test_non_process_carrier_skeletons.c
require_file docs/architecture/24-non-process-carrier-skeletons.md
require_file docs/engineering/non-process-carrier-skeletons.md
require_file docs/adr/0026-non-process-carrier-skeletons.md
require_file docs/archive/engineering/waves/spine33g-non-process-carrier-skeletons.md

for family in network_http database repository_git service endpoint socket listener model_provider review; do
  require_text system/effect/carrier_skeleton.c "$family"
  require_text cmd/yai/src/main.rs "$family"
  require_text docs/engineering/non-process-carrier-skeletons.md "$family"
done

for text in \
  "skeleton" \
  "execution_available: false" \
  "receipt_required: yes" \
  "supports_inspect: yes" \
  "carrier.v1" \
  "adapter_not_implemented" \
  "model_provider_carrier_not_implemented" \
  "review_lane_not_implemented" \
  "SPINE.93A+" \
  "yai carrier inspect database" \
  "make smoke-spine33g"
do
  require_text docs/engineering/non-process-carrier-skeletons.md "$text"
done

require_text Makefile "check-non-process-carrier-skeletons"
require_text Makefile "smoke-spine33g"
require_text include/yai/yai.h "carrier_skeleton.h"

printf 'non-process-carrier-skeletons: ok\n'
