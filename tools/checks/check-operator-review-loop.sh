#!/usr/bin/env bash
set -euo pipefail

# YAI - operator review / deferred action loop guard
#
# Purpose:
#   Keep SPINE.44A as an active control loop, not passive outcome labels.

require_file() {
  if [[ ! -f "$1" ]]; then
    printf 'missing operator review file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  local file="$1"
  local phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing operator review phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/architecture/42-operator-review-deferred-loop.md"
require_file "docs/engineering/operator-review-deferred-loop.md"
require_file "docs/adr/0042-operator-review-deferred-loop.md"
require_file "docs/archive/engineering/waves/spine44a-operator-review-deferred-loop.md"
require_file "docs/labs/filesystem-loop/README.md"
require_file "docs/labs/filesystem-loop/runbook.md"
require_file "docs/labs/filesystem-loop/prompts.json"
require_file "tests/smoke/operator-review-loop/test_operator_review_loop.sh"

for file in \
  "docs/architecture/42-operator-review-deferred-loop.md" \
  "docs/engineering/operator-review-deferred-loop.md" \
  "docs/adr/0042-operator-review-deferred-loop.md" \
  "docs/archive/engineering/waves/spine44a-operator-review-deferred-loop.md" \
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md" \
  "docs/engineering/current-status.md" \
  "docs/labs/filesystem-loop/README.md" \
  "docs/labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "require_review"
  require_phrase "$file" "pending_operator"
  require_phrase "$file" "approve"
  require_phrase "$file" "deny"
  require_phrase "$file" "defer"
  require_phrase "$file" "quarantine"
  require_phrase "$file" "carrier_attempted: false"
  require_phrase "$file" "execution_performed: false"
  require_phrase "$file" "subject:linenoise-terminal is prompt surface"
  require_phrase "$file" "operator reviewer authority"
  require_phrase "$file" "docs/labs/filesystem-loop"
done

require_phrase "cmd/yai/src/main.rs" "run-filesystem-review-loop"
require_phrase "cmd/yai/src/main.rs" "review_required: yes"
require_phrase "cmd/yai/src/main.rs" "next_commands:"
require_phrase "cmd/yai/src/main.rs" "yai control approve {review_id} --reason"
require_phrase "cmd/yai/src/main.rs" "control pending --case <case_ref>"
require_phrase "tests/smoke/operator-review-loop/test_operator_review_loop.sh" "review_required: yes"
require_phrase "tests/smoke/operator-review-loop/test_operator_review_loop.sh" "next_commands:"
require_phrase "docs/labs/filesystem-loop/prompts.json" "operator-review-pending-contract"
require_phrase "Makefile" "smoke-spine44a"

printf 'check-operator-review-loop: ok\n'
