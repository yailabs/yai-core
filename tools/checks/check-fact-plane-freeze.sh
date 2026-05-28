#!/usr/bin/env sh
set -eu

# YAI - Fact Plane Freeze guard
#
# Purpose:
#   Keep SPINE.51 as a freeze of the first DuckDB Fact Plane block: schema,
#   init, extraction, summary, report, guard, smoke and manual matrix.
#
# Non-goals:
#   No new fact tables, fact extraction classes, revision implementation,
#   retrieval quality extraction, provider runtime facts, report artifacts,
#   chart generation, reconcile action or memory consolidation.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-fact-plane-freeze: %s\n' "$1" >&2
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
  work/waves/spine51-fact-plane-freeze.md \
  work/inventories/extraction-inventory.tsv \
  cmd/yai/src/main.rs \
  tests/smoke/fact-plane-freeze/test_fact_plane_freeze.sh
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
  work/waves/spine51-fact-plane-freeze.md \
  work/inventories/extraction-inventory.tsv \
  cmd/yai/src/main.rs \
  tests/smoke/fact-plane-freeze/test_fact_plane_freeze.sh \
  Makefile \
  | tr '\n' ' ')"

for phrase in \
  "Fact Plane Freeze" \
  "yai.fact.v1" \
  "DuckDB" \
  "bitemporal" \
  "transaction_time" \
  "valid_time_start" \
  "valid_time_end" \
  "known_at" \
  "revision_of" \
  "superseded_by" \
  "retracted_by" \
  "facts_are_truth: false" \
  "memory_is_truth: false" \
  "fact_receipt" \
  "fact_decision" \
  "fact_projection" \
  "fact_carrier_outcome" \
  "fact_divergence" \
  "fact_model_behavior" \
  "fact_policy_outcome" \
  "fact_memory_quality" \
  "facts summary" \
  "facts report" \
  "none_observed" \
  "no_model_records" \
  "idempotent extraction"; do
  case "$corpus" in
    *"$phrase"*) ;;
    *) fail "missing phrase: $phrase" ;;
  esac
done

for phrase in \
  "current" \
  "superseded" \
  "retracted" \
  "stale" \
  "contested" \
  "historical_only" \
  "branch_only" \
  "counterfactual" \
  "needs_review" \
  "unknown"; do
  case "$corpus" in
    *"$phrase"*) ;;
    *) fail "missing status phrase: $phrase" ;;
  esac
done

case "$corpus" in
  *"check-fact-plane-freeze"* ) ;;
  *) fail "Makefile missing check-fact-plane-freeze" ;;
esac

case "$corpus" in
  *"smoke-spine51"* ) ;;
  *) fail "Makefile missing smoke-spine51" ;;
esac

printf 'check-fact-plane-freeze: ok\n'
