#!/usr/bin/env sh
set -eu

# YAI - fact reports CLI guard
#
# Purpose:
#   Keep SPINE.50 focused on compact read-only reports over already extracted
#   analytical facts.
#
# Non-goals:
#   No new fact extraction, audit packet generation, reconcile action or memory
#   consolidation.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-fact-reports-cli: %s\n' "$1" >&2
  exit 1
}

for file in \
  work/spines/yai-spine.md \
  work/spines/data-plane.md \
  work/spines/current-status.md \
  work/spines/command-surface.md \
  work/spines/testing.md \
  docs/architecture/data-runtime.md \
  labs/filesystem-loop/runbook.md \
  labs/filesystem-loop/tests.md \
  labs/filesystem-loop/outputs.md \
  work/waves/spine50-fact-reports-cli-validation.md \
  cmd/yai/src/main.rs \
  tests/smoke/fact-reports-cli/test_fact_reports_cli.sh
do
  [ -f "$file" ] || fail "missing file: $file"
done

corpus="$(cat \
  work/spines/yai-spine.md \
  work/spines/data-plane.md \
  work/spines/current-status.md \
  work/spines/command-surface.md \
  work/spines/testing.md \
  docs/architecture/data-runtime.md \
  labs/filesystem-loop/runbook.md \
  labs/filesystem-loop/tests.md \
  labs/filesystem-loop/outputs.md \
  work/waves/spine50-fact-reports-cli-validation.md \
  cmd/yai/src/main.rs \
  tests/smoke/fact-reports-cli/test_fact_reports_cli.sh \
  | tr '\n' ' ')"

for phrase in \
  "facts report" \
  "--section receipts" \
  "--section decisions" \
  "--section projections" \
  "--section policy" \
  "--section carriers" \
  "--section divergence" \
  "--section memory" \
  "--section model" \
  "facts reports are not truth" \
  "none_observed" \
  "no_model_records" \
  "memory_is_truth: false"; do
  case "$corpus" in
    *"$phrase"*) ;;
    *) fail "missing phrase: $phrase" ;;
  esac
done

case "$(cat Makefile)" in
  *"check-fact-reports-cli"* ) ;;
  *) fail "Makefile missing check-fact-reports-cli" ;;
esac

case "$(cat Makefile)" in
  *"smoke-spine50"* ) ;;
  *) fail "Makefile missing smoke-spine50" ;;
esac

printf 'check-fact-reports-cli: ok\n'
