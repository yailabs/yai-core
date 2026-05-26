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

require_file "work/archive/engineering-snapshots/review-loop-test-matrix.md"
require_file "work/archive/legacy-docs/engineering/waves/spine44c-review-loop-test-matrix.md"
require_file "labs/filesystem-loop/README.md"
require_file "labs/filesystem-loop/runbook.md"
require_file "labs/filesystem-loop/prompts.json"
require_file "tests/smoke/review-loop-test-matrix/test_review_loop_test_matrix.sh"

for file in \
  "work/archive/engineering-snapshots/review-loop-test-matrix.md" \
  "work/archive/legacy-docs/engineering/waves/spine44c-review-loop-test-matrix.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "work/spines/current-status.md" \
  "work/spines/yai-spine.md" \
  "work/spines/data-plane.md" \
  "work/archive/architecture-snapshots/42-operator-review-deferred-loop.md" \
  "work/archive/architecture-snapshots/43-cli-review-interaction-surface.md" \
  "labs/filesystem-loop/README.md" \
  "labs/filesystem-loop/runbook.md" \
  "labs/filesystem-loop/runbook.md"; do
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
  require_phrase "$file" "labs/filesystem-loop"
done

require_phrase "Makefile" "check-review-loop-test-matrix"
require_phrase "Makefile" "smoke-spine44c"
require_phrase "tests/smoke/review-loop-test-matrix/test_review_loop_test_matrix.sh" "review_matrix:blocked ok"
require_phrase "tests/smoke/review-loop-test-matrix/test_review_loop_test_matrix.sh" "review_matrix:non_tty ok"

printf 'check-review-loop-test-matrix: ok\n'
