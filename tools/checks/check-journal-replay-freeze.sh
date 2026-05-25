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

require_file "docs/architecture/36-journal-replay-freeze.md"
require_file "docs/engineering/journal-replay-freeze.md"
require_file "docs/adr/0036-journal-replay-freeze.md"
require_file "docs/archive/engineering/waves/spine39-journal-replay-freeze.md"
require_file "tests/smoke/journal-replay-freeze/test_journal_replay_freeze.sh"

for file in \
  "docs/architecture/36-journal-replay-freeze.md" \
  "docs/engineering/journal-replay-freeze.md" \
  "docs/adr/0036-journal-replay-freeze.md" \
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md" \
  "docs/labs/filesystem-loop/runbook.md" \
  "docs/labs/filesystem-loop/tests.md" \
  "docs/labs/filesystem-loop/outputs.md"; do
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

require_phrase "docs/engineering/journal-replay-freeze.md" "no journal fallback"
require_phrase "docs/engineering/journal-replay-freeze.md" "false completion"
require_phrase "docs/engineering/journal-replay-freeze.md" "No old docs/manuals active updates"
require_phrase "docs/labs/filesystem-loop/runbook.md" "Journal Replay Freeze"
require_phrase "docs/labs/filesystem-loop/runbook.md" "docs/labs/filesystem-loop"
require_phrase "docs/engineering/four-repo-roadmap.md" "SPINE.39  Journal Replay Freeze                                     done"
require_phrase "docs/engineering/four-repo-roadmap.md" "SPINE.40  Graph Persistence / RuntimeGraph Doctrine + Schema         done"
require_phrase "docs/engineering/data-plane-roadmap.md" "SPINE.39  Journal Replay Freeze                                     done"
require_phrase "docs/engineering/current-status.md" "SPINE.39 Journal Replay Freeze completed"
require_phrase "docs/internal/extraction-inventory.tsv" "SPINE.39"
require_phrase "Makefile" "check-journal-replay-freeze"
require_phrase "Makefile" "smoke-spine39"

if grep -RIn 'docs/manuals/manual-filesystem-loop-validation' \
  docs/engineering docs/architecture docs/labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' >/dev/null; then
  printf 'active docs contain unqualified old filesystem-loop manual path\n' >&2
  exit 1
fi
