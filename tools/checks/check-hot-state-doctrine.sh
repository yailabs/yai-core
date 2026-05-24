#!/bin/sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'missing required hot-state doctrine file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fqi "$phrase" "$ROOT/$file"; then
    printf 'missing hot-state doctrine phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/architecture/16-hot-state-plane.md"
require_file "docs/engineering/hot-state-plane.md"
require_file "docs/adr/0018-hot-state-is-not-truth.md"

require_phrase "docs/architecture/16-hot-state-plane.md" "hot state is not truth"
require_phrase "docs/architecture/16-hot-state-plane.md" "journal remains replay/audit"
require_phrase "docs/architecture/16-hot-state-plane.md" "yai.hot_state.v1"
require_phrase "docs/engineering/hot-state-plane.md" "projection freshness"
require_phrase "docs/engineering/hot-state-plane.md" "stale reason"
require_phrase "docs/engineering/hot-state-plane.md" "missing_snapshot"
require_phrase "docs/adr/0018-hot-state-is-not-truth.md" "hot state is not truth"

printf 'hot-state doctrine: ok\n'
