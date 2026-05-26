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

require_file "work/archive/architecture-snapshots/35-replay-idempotency-schema-version.md"
require_file "work/archive/engineering-snapshots/replay-idempotency-schema-version.md"
require_file "work/archive/adr/0035-replay-idempotency-schema-version.md"
require_file "work/archive/legacy-docs/engineering/waves/spine37-replay-idempotency-schema-version.md"
require_file "tests/smoke/replay-idempotency-schema-version/test_replay_idempotency_schema_version.sh"

for file in \
  "work/archive/architecture-snapshots/35-replay-idempotency-schema-version.md" \
  "work/archive/engineering-snapshots/replay-idempotency-schema-version.md" \
  "work/archive/adr/0035-replay-idempotency-schema-version.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md"; do
  require_phrase "$file" "replay cursor"
  require_phrase "$file" "journal_identity"
  require_phrase "$file" "record_schema"
  require_phrase "$file" "compatibility"
  require_phrase "$file" "cursor_line"
  require_phrase "$file" "replay-status"
done

for file in \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "filesystem-loop"
  require_phrase "$file" "journal_identity"
  require_phrase "$file" "record_schema"
  require_phrase "$file" "compatibility"
  require_phrase "$file" "cursor_line"
done

require_phrase "work/archive/engineering-snapshots/replay-idempotency-schema-version.md" "schema_mismatch"
require_phrase "work/archive/engineering-snapshots/replay-idempotency-schema-version.md" "compat_legacy"
require_phrase "work/archive/engineering-snapshots/replay-idempotency-schema-version.md" "No legacy manual path updates"
require_phrase "work/spines/yai-spine.md" "SPINE.37  Replay Idempotency + Schema Version Handling              done"
require_phrase "work/spines/yai-spine.md" "SPINE.38  Replay Diagnostics / Rebuild Report                       done"
require_phrase "work/spines/data-plane.md" "SPINE.37  Replay Idempotency + Schema Version Handling              done"
require_phrase "work/spines/current-status.md" "SPINE.37 Replay Idempotency + Schema Version Handling completed"
require_phrase "Makefile" "check-replay-idempotency-schema-version"
require_phrase "Makefile" "smoke-spine37"
