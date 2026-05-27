#!/usr/bin/env sh
set -eu

# YAI - case runtime semantics roadmap guard
#
# Purpose:
#   Keep SPINE.45B as a roadmap/doctrine rebase before bitemporal facts.
#
# Scope:
#   Checks active spine, compact architecture docs and wave record for required
#   case runtime, retrieval federation, context residency and graph revision
#   vocabulary.
#
# Non-goals:
#   Does not implement DuckDB, HNSW, external RAG, Context Compiler,
#   CaseModelSession, graph patching or counterfactual runtime behavior.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-case-runtime-semantics-roadmap: %s\n' "$1" >&2
  exit 1
}

for file in \
  work/spines/yai-spine.md \
  work/spines/data-plane.md \
  work/spines/current-status.md \
  work/spines/command-surface.md \
  work/spines/testing.md \
  docs/architecture/data-runtime.md \
  docs/architecture/model-runtime.md \
  work/waves/spine45b-case-runtime-semantics-roadmap-rebase.md
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
  docs/architecture/model-runtime.md \
  work/waves/spine45b-case-runtime-semantics-roadmap-rebase.md \
  | tr '\n' ' ')"

for phrase in \
  "bitemporal" \
  "transaction_time" \
  "valid_time_start" \
  "valid_time_end" \
  "known_at" \
  "revision_of" \
  "superseded_by" \
  "retracted_by" \
  "ContextDelta" \
  "CaseModelSession" \
  "KV cache is not YAI memory" \
  "Native vs Attached Retrieval" \
  "Retrieval Provider Contract" \
  "Retrieval Receipt" \
  "Federated Candidate Normalization" \
  "External Candidate -> Case Import" \
  "Temporal Graph Revision" \
  "Graph Patch Operation" \
  "Dependency Closure" \
  "Counterfactual" \
  "SPINE.58H" \
  "SPINE.58R" \
  "SPINE.65A" \
  "SPINE.65G" \
  "A fact is not just a row." \
  "A fact is a temporally scoped, provenance-bearing assertion." \
  "Retrieval candidates are not evidence until resolved." \
  "ContextFrame is not truth." \
  "ContextDelta is not memory." \
  "CaseModelSession is not authority."; do
  case "$corpus" in
    *"$phrase"*) ;;
    *) fail "missing phrase: $phrase" ;;
  esac
done

printf 'case_runtime_semantics_roadmap: ok\n'
