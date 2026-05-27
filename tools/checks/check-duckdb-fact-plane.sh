#!/usr/bin/env sh
set -eu

# YAI - DuckDB fact plane guard
#
# Purpose:
#   Keep SPINE.46 focused on the bitemporal DuckDB fact-plane boundary,
#   schema/status/init CLI and no-extraction posture.
#
# Scope:
#   Checks active spine, compact architecture docs, lab notes, CLI surface,
#   smoke test and wave record for required fact-plane vocabulary.
#
# Non-goals:
#   Does not extract facts, score behavior, run reports, implement HNSW,
#   implement Context Compiler or mutate operational truth.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-duckdb-fact-plane: %s\n' "$1" >&2
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
  work/waves/spine46-duckdb-fact-plane-bitemporal-schema.md \
  cmd/yai/src/main.rs \
  tests/smoke/duckdb-fact-plane/test_duckdb_fact_plane.sh
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
  work/waves/spine46-duckdb-fact-plane-bitemporal-schema.md \
  cmd/yai/src/main.rs \
  tests/smoke/duckdb-fact-plane/test_duckdb_fact_plane.sh \
  | tr '\n' ' ')"

for phrase in \
  "DuckDB" \
  "fact plane" \
  "yai.fact.v1" \
  "bitemporal" \
  "transaction_time" \
  "valid_time_start" \
  "valid_time_end" \
  "known_at" \
  "revision_of" \
  "superseded_by" \
  "retracted_by" \
  "Facts are not truth" \
  "fact_receipt" \
  "fact_decision" \
  "fact_projection" \
  "fact_carrier_outcome" \
  "fact_divergence" \
  "fact_replay" \
  "fact_runtime_graph" \
  "fact_model_behavior" \
  "fact_policy_outcome" \
  "fact_memory_quality" \
  "fact_retrieval_quality" \
  "fact_provider_runtime" \
  "yai facts status" \
  "yai facts schema" \
  "yai facts init" \
  "facts_extracted: 0" \
  "No fact extraction"; do
  case "$corpus" in
    *"$phrase"*) ;;
    *) fail "missing phrase: $phrase" ;;
  esac
done

case "$(cat Makefile)" in
  *"check-duckdb-fact-plane"* ) ;;
  *) fail "Makefile missing check-duckdb-fact-plane" ;;
esac

case "$(cat Makefile)" in
  *"smoke-spine46"* ) ;;
  *) fail "Makefile missing smoke-spine46" ;;
esac

printf 'check-duckdb-fact-plane: ok\n'
