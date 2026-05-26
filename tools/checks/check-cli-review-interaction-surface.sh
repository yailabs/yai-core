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

require_file "docs/architecture/43-cli-review-interaction-surface.md"
require_file "docs/engineering/cli-review-interaction-surface.md"
require_file "docs/adr/0043-cli-review-interaction-surface.md"
require_file "docs/archive/engineering/waves/spine44b-cli-review-interaction-surface.md"
require_file "docs/labs/filesystem-loop/README.md"
require_file "docs/labs/filesystem-loop/runbook.md"
require_file "tests/smoke/operator-review-cli/test_operator_review_cli.sh"

for file in \
  "docs/architecture/43-cli-review-interaction-surface.md" \
  "docs/engineering/cli-review-interaction-surface.md" \
  "docs/adr/0043-cli-review-interaction-surface.md" \
  "docs/archive/engineering/waves/spine44b-cli-review-interaction-surface.md" \
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md" \
  "docs/engineering/current-status.md" \
  "docs/engineering/four-repo-roadmap.md" \
  "docs/engineering/data-plane-roadmap.md" \
  "docs/labs/filesystem-loop/README.md" \
  "docs/labs/filesystem-loop/runbook.md"; do
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
  require_phrase "$file" "docs/labs/filesystem-loop"
done

require_phrase "cmd/yai/src/main.rs" "control review --case <case_ref> --interactive"
require_phrase "cmd/yai/src/main.rs" "control watch --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "control wait <review_id> --timeout <seconds>"
require_phrase "cmd/yai/src/main.rs" "next_commands:"
require_phrase "tests/smoke/operator-review-cli/test_operator_review_cli.sh" "review_cli:pending next_commands ok"
require_phrase "Makefile" "smoke-spine44b"

printf 'check-cli-review-interaction-surface: ok\n'
