#!/bin/sh
# YAI - journal replay boundary guard
#
# Purpose:
#   Ensure SPINE.35 keeps journal replay/audit separate from LMDB lookup and
#   exposes parser diagnostics before journal-to-LMDB rebuild work.
#
# Scope:
#   Checks docs, command surface, smoke target and parser-hardening terms.
#
# Non-goals:
#   Does not inspect journals, write LMDB or run smoke tests.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

require_file() {
  if [ ! -f "$ROOT/$1" ]; then
    printf 'missing journal replay boundary file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$ROOT/$file"; then
    printf 'missing journal replay boundary phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/architecture/33-journal-replay-boundary.md"
require_file "docs/engineering/journal-replay-parser-hardening.md"
require_file "docs/adr/0033-journal-replay-boundary.md"
require_file "docs/archive/engineering/waves/spine35-journal-replay-parser-hardening.md"
require_file "tests/smoke/journal-replay-boundary/test_journal_replay_boundary.sh"

for file in \
  "docs/architecture/33-journal-replay-boundary.md" \
  "docs/engineering/journal-replay-parser-hardening.md" \
  "docs/adr/0033-journal-replay-boundary.md"
do
  require_phrase "$file" "Journal is replay/audit"
  require_phrase "$file" "LMDB is durable indexed record lookup"
  require_phrase "$file" "no silent journal fallback"
  require_phrase "$file" "invalid_json"
  require_phrase "$file" "invalid_schema"
  require_phrase "$file" "unsupported_kind"
  require_phrase "$file" "duplicate"
  require_phrase "$file" "replay diagnostics"
  require_phrase "$file" "idempotency"
done

require_phrase "docs/engineering/command-surface.md" "yai journal inspect --path <journal.jsonl> [--show-errors]"
require_phrase "docs/engineering/testing.md" "make smoke-spine35"
require_phrase "docs/engineering/four-repo-roadmap.md" "SPINE.35  Journal Replay Doctrine + Parser Hardening                done"
require_phrase "docs/engineering/data-plane-roadmap.md" "SPINE.35  Journal Replay Doctrine + Parser Hardening                done"
require_phrase "docs/engineering/current-status.md" "SPINE.35 Journal Replay Doctrine + Parser Hardening completed"
require_phrase "Makefile" "check-journal-replay-boundary"
require_phrase "Makefile" "smoke-spine35"

printf 'journal-replay-boundary: ok\n'
