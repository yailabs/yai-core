#!/bin/sh
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

require_file "docs/architecture/17-lmdb-record-plane.md"
require_file "docs/engineering/lmdb-record-plane.md"
require_file "docs/adr/0019-lmdb-record-plane.md"
require_file "docs/archive/engineering/waves/spine29-lmdb-record-plane-doctrine-schema.md"
require_file "docs/engineering/data-plane-roadmap.md"
require_file "docs/engineering/command-surface.md"

for file in \
  "docs/architecture/17-lmdb-record-plane.md" \
  "docs/engineering/lmdb-record-plane.md" \
  "docs/adr/0019-lmdb-record-plane.md"
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

require_phrase "docs/architecture/17-lmdb-record-plane.md" "SPINE.30 LMDB Record Write Path"
require_phrase "docs/engineering/data-plane-roadmap.md" "SPINE.30 LMDB Record Write Path"
require_phrase "docs/engineering/command-surface.md" "yai store status"
require_phrase "docs/engineering/command-surface.md" "record_store_status"

printf 'lmdb-record-plane doctrine: ok\n'
