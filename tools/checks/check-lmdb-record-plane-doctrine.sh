#!/bin/sh
# YAI - LMDB record-plane doctrine guard
#
# Purpose:
#   Ensure active docs preserve LMDB as durable indexed record lookup.
#
# Scope:
#   Checks LMDB record-plane docs, ADR and command surface references.
#
# Non-goals:
#   Does not open LMDB or validate record contents.
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'missing LMDB record-plane file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fqi "$phrase" "$ROOT/$file"; then
    printf 'missing LMDB record-plane phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/17-lmdb-record-plane.md"
require_file "work/archive/engineering-snapshots/lmdb-record-plane.md"
require_file "work/archive/adr/0019-lmdb-record-plane.md"
require_file "work/archive/legacy-docs/engineering/waves/spine29-lmdb-record-plane-doctrine-schema.md"
require_file "work/spines/data-plane.md"
require_file "work/spines/command-surface.md"

for file in \
  "work/archive/architecture-snapshots/17-lmdb-record-plane.md" \
  "work/archive/engineering-snapshots/lmdb-record-plane.md" \
  "work/archive/adr/0019-lmdb-record-plane.md"
do
  require_phrase "$file" "LMDB is durable indexed record lookup"
  require_phrase "$file" "hot state is not LMDB"
  require_phrase "$file" "journal remains replay/audit"
  require_phrase "$file" "record:id"
  require_phrase "$file" "record:case"
  require_phrase "$file" "record:kind"
  require_phrase "$file" "record:subject"
  require_phrase "$file" "yai.record.v1"
done

require_phrase "work/archive/architecture-snapshots/17-lmdb-record-plane.md" "SPINE.30 LMDB Record Write Path"
require_phrase "work/spines/data-plane.md" "SPINE.30 LMDB Record Write Path"
require_phrase "work/spines/command-surface.md" "yai store status"
require_phrase "work/spines/command-surface.md" "record_store_status"

printf 'lmdb-record-plane doctrine: ok\n'
