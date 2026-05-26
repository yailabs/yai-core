#!/usr/bin/env sh
set -eu

# YAI - documentation root canon guard
#
# Purpose:
#   Ensure docs/ stays curated, while labs/ and work/ own execution material.
#
# Scope:
#   Checks active root layout and active references to old documentation roots.
#
# Non-goals:
#   Does not validate historical archive content.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-doc-root-canon: %s\n' "$1" >&2
  exit 1
}

require_file() {
  [ -f "$1" ] || fail "missing file: $1"
}

require_dir() {
  [ -d "$1" ] || fail "missing directory: $1"
}

for forbidden in \
  docs/internal \
  docs/engineering \
  docs/spines \
  docs/product \
  docs/reference \
  docs/status \
  docs/protocols \
  docs/adr \
  docs/labs \
  docs/lab-standards
do
  [ ! -e "$forbidden" ] || fail "active forbidden path exists: $forbidden"
done

require_file docs/architecture/system.md
require_file docs/architecture/data-runtime.md
require_file docs/architecture/control-runtime.md
require_file docs/architecture/model-runtime.md
require_file docs/architecture/protocols.md
require_file docs/architecture/decisions.md
require_dir labs
require_dir labs/standards
require_dir work
require_dir work/spines
require_file work/inventories/extraction-inventory.tsv

active_files="$(
  {
    printf '%s\n' Makefile README.md
    find docs labs work/spines work/waves work/agents tools tests configs cmd net -type f \
      ! -path 'labs/*/runs/*' \
      ! -path 'work/archive/*' \
      2>/dev/null
  } | sort -u
)"

if [ -n "$active_files" ]; then
  if printf '%s\n' "$active_files" | xargs grep -nE \
    '(^|[^A-Za-z0-9_-])docs/(internal/extraction-inventory\.tsv|labs/|lab-standards/|engineering/|spines/|manuals/)' \
    >/tmp/yai-doc-root-canon-refs.$$ 2>/dev/null; then
    cat /tmp/yai-doc-root-canon-refs.$$ >&2
    rm -f /tmp/yai-doc-root-canon-refs.$$
    fail "active references to old documentation roots remain"
  fi
  rm -f /tmp/yai-doc-root-canon-refs.$$
fi

printf 'doc_root_canon: ok\n'
