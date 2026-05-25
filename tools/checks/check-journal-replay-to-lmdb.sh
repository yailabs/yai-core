#!/bin/sh
# YAI - journal replay to LMDB guard
#
# Purpose:
#   Ensure SPINE.36 keeps journal replay diagnostic, idempotent and backed by
#   the frozen LMDB record-plane writer.
#
# Scope:
#   Checks docs, lab runbook, command surface, guard and smoke target.
#
# Non-goals:
#   Does not replay a journal or open LMDB.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'missing journal replay to LMDB file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$ROOT/$file"; then
    printf 'missing journal replay to LMDB phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/architecture/34-journal-replay-to-lmdb.md"
require_file "docs/engineering/journal-replay-to-lmdb.md"
require_file "docs/adr/0034-journal-replay-to-lmdb.md"
require_file "docs/archive/engineering/waves/spine36-journal-replay-to-lmdb.md"
require_file "tests/smoke/journal-replay-to-lmdb/test_journal_replay_to_lmdb.sh"
require_file "docs/labs/filesystem-loop/runbook.md"
require_file "docs/labs/filesystem-loop/tests.md"
require_file "docs/labs/filesystem-loop/outputs.md"

for file in \
  "docs/architecture/34-journal-replay-to-lmdb.md" \
  "docs/engineering/journal-replay-to-lmdb.md" \
  "docs/adr/0034-journal-replay-to-lmdb.md"
do
  require_phrase "$file" "journal replay to LMDB"
  require_phrase "$file" "idempotent replay"
  require_phrase "$file" "dry-run"
  require_phrase "$file" "records_written"
  require_phrase "$file" "records_duplicate"
  require_phrase "$file" "invalid_entries"
  require_phrase "$file" "no silent skip"
  require_phrase "$file" "no journal fallback"
done

require_phrase "docs/engineering/command-surface.md" "yai journal replay --path <journal.jsonl> [--dry-run]"
require_phrase "docs/engineering/testing.md" "make smoke-spine36"
require_phrase "docs/engineering/four-repo-roadmap.md" "SPINE.36  Journal Replay to LMDB                                    done"
require_phrase "docs/engineering/data-plane-roadmap.md" "SPINE.36  Journal Replay to LMDB                                    done"
require_phrase "docs/engineering/current-status.md" "SPINE.36 Journal Replay to LMDB completed"
require_phrase "docs/labs/filesystem-loop/runbook.md" "Journal Replay to LMDB"
require_phrase "docs/labs/filesystem-loop/tests.md" "journal replay to LMDB"
require_phrase "docs/labs/filesystem-loop/outputs.md" "journal replay to LMDB"
require_phrase "Makefile" "check-journal-replay-to-lmdb"
require_phrase "Makefile" "smoke-spine36"

printf 'journal-replay-to-lmdb: ok\n'
