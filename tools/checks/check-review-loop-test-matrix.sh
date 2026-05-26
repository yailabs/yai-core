#!/usr/bin/env bash
set -euo pipefail

# YAI - review loop test matrix guard
#
# Purpose:
#   Keep SPINE.44C focused on reproducible review/defer/quarantine tests and
#   lab alignment, including the model proposal boundary.

require_file() {
  if [[ ! -f "$1" ]]; then
    printf 'missing review matrix file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  local file="$1"
  local phrase="$2"
  local normalized
  normalized="$(tr '\n' ' ' < "$file")"
  if [[ "$normalized" != *"$phrase"* ]]; then
    printf 'missing review matrix phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/engineering/review-loop-test-matrix.md"
require_file "docs/archive/engineering/waves/spine44c-review-loop-test-matrix.md"
require_file "docs/labs/filesystem-loop/README.md"
require_file "docs/labs/filesystem-loop/runbook.md"
require_file "docs/labs/filesystem-loop/prompts.json"
require_file "tests/smoke/review-loop-test-matrix/test_review_loop_test_matrix.sh"

for file in \
  "docs/engineering/review-loop-test-matrix.md" \
  "docs/archive/engineering/waves/spine44c-review-loop-test-matrix.md" \
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md" \
  "docs/engineering/current-status.md" \
  "docs/engineering/four-repo-roadmap.md" \
  "docs/engineering/data-plane-roadmap.md" \
  "docs/architecture/42-operator-review-deferred-loop.md" \
  "docs/architecture/43-cli-review-interaction-surface.md" \
  "docs/labs/filesystem-loop/README.md" \
  "docs/labs/filesystem-loop/runbook.md" \
  "docs/labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "approve"
  require_phrase "$file" "deny"
  require_phrase "$file" "defer"
  require_phrase "$file" "quarantine"
  require_phrase "$file" "pending_operator"
  require_phrase "$file" "next_commands"
  require_phrase "$file" "carrier_attempted: false"
  require_phrase "$file" "execution_performed: false"
  require_phrase "$file" "wait timeout"
  require_phrase "$file" "watch"
  require_phrase "$file" "model proposal observed"
  require_phrase "$file" "model cannot approve"
  require_phrase "$file" "automatic proposed-op gate import is future work"
  require_phrase "$file" "docs/labs/filesystem-loop"
done

require_phrase "Makefile" "check-review-loop-test-matrix"
require_phrase "Makefile" "smoke-spine44c"
require_phrase "tests/smoke/review-loop-test-matrix/test_review_loop_test_matrix.sh" "review_matrix:blocked ok"
require_phrase "tests/smoke/review-loop-test-matrix/test_review_loop_test_matrix.sh" "review_matrix:non_tty ok"

printf 'check-review-loop-test-matrix: ok\n'
