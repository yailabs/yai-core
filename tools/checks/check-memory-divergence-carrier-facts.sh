#!/usr/bin/env sh
set -eu

# YAI - memory/divergence/carrier facts guard
#
# Purpose:
#   Keep SPINE.49 focused on analytical carrier, divergence and memory quality
#   facts derived from LMDB records.
#
# Non-goals:
#   No memory consolidation, graph patching, reconcile action, carrier behavior
#   change or divergence generation change.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-memory-divergence-carrier-facts: %s\n' "$1" >&2
  exit 1
}

for file in \
  work/spines/yai-spine.md \
  work/spines/data-plane.md \
  work/spines/current-status.md \
  work/spines/command-surface.md \
  work/spines/testing.md \
  docs/architecture/data-runtime.md \
  docs/architecture/control-runtime.md \
  labs/filesystem-loop/runbook.md \
  labs/filesystem-loop/tests.md \
  labs/filesystem-loop/outputs.md \
  work/waves/spine49-memory-divergence-carrier-facts.md \
  cmd/yai/src/main.rs \
  tests/smoke/memory-divergence-carrier-facts/test_memory_divergence_carrier_facts.sh
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
  docs/architecture/control-runtime.md \
  labs/filesystem-loop/runbook.md \
  labs/filesystem-loop/tests.md \
  labs/filesystem-loop/outputs.md \
  work/waves/spine49-memory-divergence-carrier-facts.md \
  cmd/yai/src/main.rs \
  tests/smoke/memory-divergence-carrier-facts/test_memory_divergence_carrier_facts.sh \
  | tr '\n' ' ')"

for phrase in \
  "fact_carrier_outcome" \
  "fact_divergence" \
  "fact_memory_quality" \
  "carrier_outcome" \
  "divergence" \
  "memory_candidate" \
  "memory facts are not memory" \
  "divergence facts are not reconcile action" \
  "carrier facts measure carrier posture" \
  "facts are not truth" \
  "bitemporal" \
  "idempotent extraction" \
  "memory_is_truth: false" \
  "kind carrier_outcome" \
  "kind divergence" \
  "kind memory_quality" \
  "kind operational"; do
  case "$corpus" in
    *"$phrase"*) ;;
    *) fail "missing phrase: $phrase" ;;
  esac
done

case "$(cat Makefile)" in
  *"check-memory-divergence-carrier-facts"* ) ;;
  *) fail "Makefile missing check-memory-divergence-carrier-facts" ;;
esac

case "$(cat Makefile)" in
  *"smoke-spine49"* ) ;;
  *) fail "Makefile missing smoke-spine49" ;;
esac

printf 'check-memory-divergence-carrier-facts: ok\n'
