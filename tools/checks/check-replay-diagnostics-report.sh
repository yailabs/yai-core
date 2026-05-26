#!/bin/sh
# YAI - replay diagnostics report guard
#
# Purpose:
#   Ensure SPINE.38 keeps durable replay report documentation and smoke coverage
#   active.
#
# Scope:
#   Text guard only. Does not open LMDB or execute replay.
set -eu

require_file() {
  if [ ! -f "$1" ]; then
    printf 'missing replay diagnostics report file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing replay diagnostics report phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/34-replay-diagnostics-rebuild-report.md"
require_file "work/archive/engineering-snapshots/replay-diagnostics-rebuild-report.md"
require_file "work/archive/adr/0034-replay-diagnostics-rebuild-report.md"
require_file "work/archive/legacy-docs/engineering/waves/spine38-replay-diagnostics-rebuild-report.md"
require_file "tests/smoke/replay-diagnostics-report/test_replay_diagnostics_report.sh"

for file in \
  "work/archive/architecture-snapshots/34-replay-diagnostics-rebuild-report.md" \
  "work/archive/engineering-snapshots/replay-diagnostics-rebuild-report.md" \
  "work/archive/adr/0034-replay-diagnostics-rebuild-report.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "yai.replay_report.v1"
  require_phrase "$file" "replay-report"
  require_phrase "$file" "journal_identity"
  require_phrase "$file" "compatibility"
  require_phrase "$file" "cursor_line"
  require_phrase "$file" "records_written"
  require_phrase "$file" "records_duplicate"
  require_phrase "$file" "invalid_entries"
  require_phrase "$file" "failed report"
done

require_phrase "labs/filesystem-loop/runbook.md" "labs/filesystem-loop"
require_phrase "work/inventories/extraction-inventory.tsv" "wave=SPINE.38"
require_phrase "work/inventories/extraction-inventory.tsv" "SPINE.38"
require_phrase "Makefile" "check-replay-diagnostics-report"
require_phrase "Makefile" "smoke-spine38"
