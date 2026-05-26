#!/bin/sh
# YAI - journal replay freeze guard
#
# Purpose:
#   Ensure SPINE.39 freezes the replay block as an inspectable, diagnostic,
#   idempotent and report-producing materialization path.
#
# Scope:
#   Text guard only. Does not open LMDB or execute replay.
set -eu

require_file() {
  if [ ! -f "$1" ]; then
    printf 'missing journal replay freeze file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing journal replay freeze phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/36-journal-replay-freeze.md"
require_file "work/archive/engineering-snapshots/journal-replay-freeze.md"
require_file "work/archive/adr/0036-journal-replay-freeze.md"
require_file "work/archive/legacy-docs/engineering/waves/spine39-journal-replay-freeze.md"
require_file "tests/smoke/journal-replay-freeze/test_journal_replay_freeze.sh"

for file in \
  "work/archive/architecture-snapshots/36-journal-replay-freeze.md" \
  "work/archive/engineering-snapshots/journal-replay-freeze.md" \
  "work/archive/adr/0036-journal-replay-freeze.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "journal inspect"
  require_phrase "$file" "journal replay"
  require_phrase "$file" "replay-status"
  require_phrase "$file" "replay-report"
  require_phrase "$file" "yai.replay_report.v1"
  require_phrase "$file" "journal_identity"
  require_phrase "$file" "cursor_line"
  require_phrase "$file" "compatibility"
  require_phrase "$file" "idempotent"
  require_phrase "$file" "schema_mismatch"
  require_phrase "$file" "invalid_json"
done

require_phrase "work/archive/engineering-snapshots/journal-replay-freeze.md" "no journal fallback"
require_phrase "work/archive/engineering-snapshots/journal-replay-freeze.md" "false completion"
require_phrase "work/archive/engineering-snapshots/journal-replay-freeze.md" "No legacy manual active updates"
require_phrase "labs/filesystem-loop/runbook.md" "Journal Replay Freeze"
require_phrase "labs/filesystem-loop/runbook.md" "labs/filesystem-loop"
require_phrase "work/spines/yai-spine.md" "SPINE.39  Journal Replay Freeze                                     done"
require_phrase "work/spines/yai-spine.md" "SPINE.40  Graph Persistence / RuntimeGraph Doctrine + Schema         done"
require_phrase "work/spines/data-plane.md" "SPINE.39  Journal Replay Freeze                                     done"
require_phrase "work/spines/current-status.md" "SPINE.39 Journal Replay Freeze completed"
require_phrase "work/inventories/extraction-inventory.tsv" "SPINE.39"
require_phrase "Makefile" "check-journal-replay-freeze"
require_phrase "Makefile" "smoke-spine39"

if grep -RIn 'historical filesystem-loop manual' \
  docs/engineering docs/architecture docs/labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' |
  grep -v 'pre-redirect' >/dev/null; then
  printf 'active docs contain unqualified legacy filesystem-loop manual path\n' >&2
  exit 1
fi
