#!/bin/sh
# YAI - replay idempotency schema version guard
#
# Purpose:
#   Ensure SPINE.37 keeps replay cursor, schema compatibility and replay-status
#   documentation active.
#
# Scope:
#   Text guard only. Does not open LMDB or execute replay.
set -eu

require_file() {
  if [ ! -f "$1" ]; then
    printf 'missing replay idempotency schema file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing replay idempotency schema phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/architecture/35-replay-idempotency-schema-version.md"
require_file "docs/engineering/replay-idempotency-schema-version.md"
require_file "docs/adr/0035-replay-idempotency-schema-version.md"
require_file "docs/archive/engineering/waves/spine37-replay-idempotency-schema-version.md"
require_file "tests/smoke/replay-idempotency-schema-version/test_replay_idempotency_schema_version.sh"

for file in \
  "docs/architecture/35-replay-idempotency-schema-version.md" \
  "docs/engineering/replay-idempotency-schema-version.md" \
  "docs/adr/0035-replay-idempotency-schema-version.md" \
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md"; do
  require_phrase "$file" "replay cursor"
  require_phrase "$file" "journal_identity"
  require_phrase "$file" "record_schema"
  require_phrase "$file" "compatibility"
  require_phrase "$file" "cursor_line"
  require_phrase "$file" "replay-status"
done

for file in \
  "docs/labs/filesystem-loop/runbook.md" \
  "docs/labs/filesystem-loop/tests.md" \
  "docs/labs/filesystem-loop/outputs.md"; do
  require_phrase "$file" "filesystem-loop"
  require_phrase "$file" "journal_identity"
  require_phrase "$file" "record_schema"
  require_phrase "$file" "compatibility"
  require_phrase "$file" "cursor_line"
done

require_phrase "docs/engineering/replay-idempotency-schema-version.md" "schema_mismatch"
require_phrase "docs/engineering/replay-idempotency-schema-version.md" "compat_legacy"
require_phrase "docs/engineering/replay-idempotency-schema-version.md" "No old docs/manuals path updates"
require_phrase "docs/engineering/four-repo-roadmap.md" "SPINE.37  Replay Idempotency + Schema Version Handling              done"
require_phrase "docs/engineering/four-repo-roadmap.md" "SPINE.38  Replay Diagnostics / Rebuild Report                       done"
require_phrase "docs/engineering/data-plane-roadmap.md" "SPINE.37  Replay Idempotency + Schema Version Handling              done"
require_phrase "docs/engineering/current-status.md" "SPINE.37 Replay Idempotency + Schema Version Handling completed"
require_phrase "Makefile" "check-replay-idempotency-schema-version"
require_phrase "Makefile" "smoke-spine37"
