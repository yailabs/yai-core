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

require_file "docs/architecture/16-hot-state-plane.md"
require_file "docs/engineering/hot-state-plane.md"
require_file "docs/engineering/data-plane-roadmap.md"
require_file "docs/engineering/command-surface.md"
require_file "docs/engineering/testing.md"
require_file "docs/archive/engineering/waves/spine28-hot-state-freeze.md"

require_phrase "docs/architecture/16-hot-state-plane.md" "SPINE.28 freezes"
require_phrase "docs/architecture/16-hot-state-plane.md" "hot state is not truth"
require_phrase "docs/architecture/16-hot-state-plane.md" "journal remains replay/audit"
require_phrase "docs/architecture/16-hot-state-plane.md" "yai.hot_state.v1"
require_phrase "docs/architecture/16-hot-state-plane.md" "missing_snapshot"
require_phrase "docs/architecture/16-hot-state-plane.md" "invalid_snapshot"
require_phrase "docs/architecture/16-hot-state-plane.md" "case_session_status"
require_phrase "docs/architecture/16-hot-state-plane.md" "case_context_status"
require_phrase "docs/architecture/16-hot-state-plane.md" "projection_freshness"

require_phrase "docs/engineering/hot-state-plane.md" "SPINE.28 freezes"
require_phrase "docs/engineering/hot-state-plane.md" "freshness_policy"
require_phrase "docs/engineering/hot-state-plane.md" "case_session"
require_phrase "docs/engineering/hot-state-plane.md" "case_context"
require_phrase "docs/engineering/hot-state-plane.md" "LMDB will not replace hot state"

require_phrase "docs/engineering/data-plane-roadmap.md" "Hot State Freeze"
require_phrase "docs/engineering/data-plane-roadmap.md" "SPINE.29 begins the durable record plane"
require_phrase "docs/engineering/data-plane-roadmap.md" "LMDB will not replace hot state"

require_phrase "docs/engineering/command-surface.md" "yai hot status"
require_phrase "docs/engineering/command-surface.md" "yai doctor"
require_phrase "docs/engineering/command-surface.md" "yai.hot_state.v1"
require_phrase "docs/engineering/command-surface.md" "missing_snapshot"
require_phrase "docs/engineering/command-surface.md" "invalid_snapshot"

require_phrase "docs/engineering/testing.md" "SPINE.28 Hot State Freeze Loop"
require_phrase "docs/engineering/testing.md" "make check-hot-state-freeze"
require_phrase "docs/engineering/testing.md" "smoke-spine23"
require_phrase "docs/engineering/testing.md" "smoke-spine27"

printf 'hot-state freeze: ok\n'
