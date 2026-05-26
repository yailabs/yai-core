#!/bin/sh
# YAI - hot-state freeze guard
#
# Purpose:
#   Ensure the frozen hot-state docs and command surface remain present.
#
# Scope:
#   Checks active docs for freeze phrases and command/status references.
#
# Non-goals:
#   Does not run daemon hot-state smoke tests.
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'missing hot-state freeze file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fqi "$phrase" "$ROOT/$file"; then
    printf 'missing hot-state freeze phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/16-hot-state-plane.md"
require_file "work/archive/engineering-snapshots/hot-state-plane.md"
require_file "work/spines/data-plane.md"
require_file "work/spines/command-surface.md"
require_file "work/spines/testing.md"
require_file "work/archive/legacy-docs/engineering/waves/spine28-hot-state-freeze.md"

require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "SPINE.28 freezes"
require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "hot state is not truth"
require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "journal remains replay/audit"
require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "yai.hot_state.v1"
require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "missing_snapshot"
require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "invalid_snapshot"
require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "case_session_status"
require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "case_context_status"
require_phrase "work/archive/architecture-snapshots/16-hot-state-plane.md" "projection_freshness"

require_phrase "work/archive/engineering-snapshots/hot-state-plane.md" "SPINE.28 freezes"
require_phrase "work/archive/engineering-snapshots/hot-state-plane.md" "freshness_policy"
require_phrase "work/archive/engineering-snapshots/hot-state-plane.md" "case_session"
require_phrase "work/archive/engineering-snapshots/hot-state-plane.md" "case_context"
require_phrase "work/archive/engineering-snapshots/hot-state-plane.md" "LMDB will not replace hot state"

require_phrase "work/spines/data-plane.md" "Hot State Freeze"
require_phrase "work/spines/data-plane.md" "SPINE.29 begins the durable record plane"
require_phrase "work/spines/data-plane.md" "LMDB will not replace hot state"

require_phrase "work/spines/command-surface.md" "yai hot status"
require_phrase "work/spines/command-surface.md" "yai doctor"
require_phrase "work/spines/command-surface.md" "yai.hot_state.v1"
require_phrase "work/spines/command-surface.md" "missing_snapshot"
require_phrase "work/spines/command-surface.md" "invalid_snapshot"

require_phrase "work/spines/testing.md" "SPINE.28 Hot State Freeze Loop"
require_phrase "work/spines/testing.md" "make check-hot-state-freeze"
require_phrase "work/spines/testing.md" "smoke-spine23"
require_phrase "work/spines/testing.md" "smoke-spine27"

printf 'hot-state freeze: ok\n'
