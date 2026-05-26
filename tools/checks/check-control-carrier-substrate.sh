#!/bin/sh
# YAI - control/carrier substrate guard
#
# Purpose:
#   Ensure SPINE.33A doctrine and ABI anchors stay visible.
#
# Scope:
#   Checks active docs and headers for control/carrier substrate boundaries.
#
# Non-goals:
#   Does not prove carrier execution or dispatch runtime behavior.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

require_text() {
  file="$1"
  text="$2"
  if ! grep -Fqi "$text" "$ROOT/$file"; then
    printf 'missing required text in %s: %s\n' "$file" "$text" >&2
    exit 1
  fi
}

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'required control/carrier file missing: %s\n' "$1" >&2
    exit 1
  fi
}

require_file "include/yai/effect/carrier_family.h"
require_file "include/yai/effect/dispatch.h"
require_file "include/yai/effect/receipt_guarantee.h"
require_file "include/yai/control/gate_outcome.h"
require_file "include/yai/observation/host_observation.h"
require_file "work/archive/architecture-snapshots/18-control-carrier-substrate.md"
require_file "work/archive/engineering-snapshots/control-carrier-rebase.md"
require_file "work/archive/adr/0020-control-carrier-substrate.md"

for file in \
  "work/archive/architecture-snapshots/18-control-carrier-substrate.md" \
  "work/archive/engineering-snapshots/control-carrier-rebase.md" \
  "work/archive/adr/0020-control-carrier-substrate.md"
do
  require_text "$file" "carrier family"
  require_text "$file" "gate outcome"
  require_text "$file" "dispatch status"
  require_text "$file" "receipt guarantee mode"
  require_text "$file" "observation is not enforcement"
  require_text "$file" "decision is not execution"
  require_text "$file" "carrier is not free tool invocation"
  require_text "$file" "receipt is not logging"
done

printf 'control-carrier-substrate: ok\n'
