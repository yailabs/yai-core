#!/usr/bin/env sh
set -eu

# YAI - receipt/decision/projection facts guard
#
# Purpose:
#   Keep SPINE.47 focused on idempotent analytical fact extraction from LMDB
#   records into DuckDB fact_receipt, fact_decision and fact_projection.
#
# Scope:
#   Checks active spine, architecture docs, lab notes, CLI surface, smoke test
#   and wave record for required fact extraction vocabulary.
#
# Non-goals:
#   Does not implement model behavior facts, policy outcome facts, memory
#   quality facts, retrieval quality facts, graph patching or fact revision.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-receipt-decision-projection-facts: %s\n' "$1" >&2
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
  work/waves/spine47-receipt-decision-projection-facts.md \
  cmd/yai/src/main.rs \
  tests/smoke/receipt-decision-projection-facts/test_receipt_decision_projection_facts.sh
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
  work/waves/spine47-receipt-decision-projection-facts.md \
  cmd/yai/src/main.rs \
  tests/smoke/receipt-decision-projection-facts/test_receipt_decision_projection_facts.sh \
  | tr '\n' ' ')"

for phrase in \
  "fact_receipt" \
  "fact_decision" \
  "fact_projection" \
  "facts extract" \
  "facts summary" \
  "idempotent extraction" \
  "Facts are not truth" \
  "transaction_time" \
  "valid_time_start" \
  "valid_time_end" \
  "known_at" \
  "revision fields" \
  "revision_of" \
  "superseded_by" \
  "retracted_by" \
  "deterministic fact IDs" \
  "valid_time_end sentinel: 0" \
  "fact:<kind>:<source_record_id>" \
  "No fact revision"; do
  case "$corpus" in
    *"$phrase"*) ;;
    *) fail "missing phrase: $phrase" ;;
  esac
done

case "$(cat Makefile)" in
  *"check-receipt-decision-projection-facts"* ) ;;
  *) fail "Makefile missing check-receipt-decision-projection-facts" ;;
esac

case "$(cat Makefile)" in
  *"smoke-spine47"* ) ;;
  *) fail "Makefile missing smoke-spine47" ;;
esac

printf 'check-receipt-decision-projection-facts: ok\n'
