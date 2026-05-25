#!/bin/sh
# YAI - LMDB record-plane freeze guard
#
# Purpose:
#   Ensure SPINE.34 freezes the LMDB record plane as durable indexed lookup.
#
# Scope:
#   Checks docs, command surface, smoke target and record-plane boundaries.
#
# Non-goals:
#   Does not open LMDB, run smoke tests or validate journal replay.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'missing LMDB record-plane freeze file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$ROOT/$file"; then
    printf 'missing LMDB record-plane freeze phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/architecture/32-lmdb-record-plane-freeze.md"
require_file "docs/engineering/lmdb-record-plane-freeze.md"
require_file "docs/adr/0032-lmdb-record-plane-freeze.md"
require_file "docs/archive/engineering/waves/spine34-lmdb-record-plane-freeze.md"
require_file "tests/smoke/record-store-freeze/test_record_store_freeze.sh"

for file in \
  "docs/architecture/32-lmdb-record-plane-freeze.md" \
  "docs/engineering/lmdb-record-plane-freeze.md" \
  "docs/adr/0032-lmdb-record-plane-freeze.md"
do
  require_phrase "$file" "SPINE.34 LMDB Record Plane Freeze"
  require_phrase "$file" "LMDB is durable indexed record lookup"
  require_phrase "$file" "journal remains replay/audit"
  require_phrase "$file" "no journal fallback"
  require_phrase "$file" "records_by_id"
  require_phrase "$file" "records_by_case"
  require_phrase "$file" "records_by_kind"
  require_phrase "$file" "records_by_subject"
  require_phrase "$file" "records_by_receipt"
  require_phrase "$file" "carrier_request"
  require_phrase "$file" "effect_receipt"
  require_phrase "$file" "divergence"
done

require_phrase "docs/engineering/four-repo-roadmap.md" "SPINE.34  LMDB Record Plane Freeze                                  done"
require_phrase "docs/engineering/data-plane-roadmap.md" "SPINE.34  LMDB Record Plane Freeze                                  done"
require_phrase "docs/engineering/current-status.md" "SPINE.34 LMDB Record Plane Freeze completed"
require_phrase "docs/engineering/command-surface.md" "SPINE.34 adds no new command"
require_phrase "docs/engineering/testing.md" "make smoke-spine34"
require_phrase "Makefile" "check-lmdb-record-plane-freeze"
require_phrase "Makefile" "smoke-spine34"

printf 'lmdb-record-plane-freeze: ok\n'
