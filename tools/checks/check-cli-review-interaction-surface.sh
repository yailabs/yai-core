#!/usr/bin/env bash
set -euo pipefail

# YAI - CLI review interaction surface guard
#
# Purpose:
#   Keep SPINE.44B as a practical CLI/automation surface over durable review
#   state, without turning the backend into a terminal prompt.

require_file() {
  if [[ ! -f "$1" ]]; then
    printf 'missing cli review file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  local file="$1"
  local phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing cli review phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/43-cli-review-interaction-surface.md"
require_file "work/archive/engineering-snapshots/cli-review-interaction-surface.md"
require_file "work/archive/adr/0043-cli-review-interaction-surface.md"
require_file "work/archive/legacy-docs/engineering/waves/spine44b-cli-review-interaction-surface.md"
require_file "labs/filesystem-loop/README.md"
require_file "labs/filesystem-loop/runbook.md"
require_file "tests/smoke/operator-review-cli/test_operator_review_cli.sh"

for file in \
  "work/archive/architecture-snapshots/43-cli-review-interaction-surface.md" \
  "work/archive/engineering-snapshots/cli-review-interaction-surface.md" \
  "work/archive/adr/0043-cli-review-interaction-surface.md" \
  "work/archive/legacy-docs/engineering/waves/spine44b-cli-review-interaction-surface.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "work/spines/current-status.md" \
  "work/spines/yai-spine.md" \
  "work/spines/data-plane.md" \
  "labs/filesystem-loop/README.md" \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "control pending"
  require_phrase "$file" "control show"
  require_phrase "$file" "control review --interactive"
  require_phrase "$file" "control watch"
  require_phrase "$file" "control wait"
  require_phrase "$file" "next_commands"
  require_phrase "$file" "not_a_tty"
  require_phrase "$file" "pending_operator"
  require_phrase "$file" "approve"
  require_phrase "$file" "deny"
  require_phrase "$file" "defer"
  require_phrase "$file" "quarantine"
  require_phrase "$file" "carrier_attempted: false"
  require_phrase "$file" "execution_performed: false"
  require_phrase "$file" "subject:linenoise-terminal is prompt surface"
  require_phrase "$file" "subject:operator-reviewer is review authority"
  require_phrase "$file" "labs/filesystem-loop"
done

require_phrase "cmd/yai/src/main.rs" "control review --case <case_ref> --interactive"
require_phrase "cmd/yai/src/main.rs" "control watch --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "control wait <review_id> --timeout <seconds>"
require_phrase "cmd/yai/src/main.rs" "next_commands:"
require_phrase "tests/smoke/operator-review-cli/test_operator_review_cli.sh" "review_cli:pending next_commands ok"
require_phrase "Makefile" "smoke-spine44b"

printf 'check-cli-review-interaction-surface: ok\n'
