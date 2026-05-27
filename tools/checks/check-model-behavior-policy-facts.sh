#!/usr/bin/env sh
set -eu

# YAI - model behavior / policy outcome facts guard
#
# Purpose:
#   Keep SPINE.48 focused on analytical facts for model behavior and
#   policy/control posture.
#
# Non-goals:
#   No model invocation, no LLM-based classification, no policy behavior
#   change and no automatic proposed-op import.

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-model-behavior-policy-facts: %s\n' "$1" >&2
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
  labs/filesystem-loop/runbook.md \
  labs/filesystem-loop/tests.md \
  labs/filesystem-loop/outputs.md \
  work/waves/spine48-model-behavior-policy-outcome-facts.md \
  cmd/yai/src/main.rs \
  tests/smoke/model-behavior-policy-facts/test_model_behavior_policy_facts.sh
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
  labs/filesystem-loop/runbook.md \
  labs/filesystem-loop/tests.md \
  labs/filesystem-loop/outputs.md \
  work/waves/spine48-model-behavior-policy-outcome-facts.md \
  cmd/yai/src/main.rs \
  tests/smoke/model-behavior-policy-facts/test_model_behavior_policy_facts.sh \
  | tr '\n' ' ')"

for phrase in \
  "fact_model_behavior" \
  "fact_policy_outcome" \
  "model proposal is not execution" \
  "model cannot approve" \
  "automatic proposed-op gate import is future work" \
  "facts are not truth" \
  "authority_overclaim" \
  "unsupported_claim" \
  "review_required" \
  "policy_outcome" \
  "bitemporal" \
  "No LLM-based classification" \
  "facts extract --case" \
  "kind model_behavior" \
  "kind policy_outcome" \
  "kind behavior"; do
  case "$corpus" in
    *"$phrase"*) ;;
    *) fail "missing phrase: $phrase" ;;
  esac
done

case "$(cat Makefile)" in
  *"check-model-behavior-policy-facts"* ) ;;
  *) fail "Makefile missing check-model-behavior-policy-facts" ;;
esac

case "$(cat Makefile)" in
  *"smoke-spine48"* ) ;;
  *) fail "Makefile missing smoke-spine48" ;;
esac

printf 'check-model-behavior-policy-facts: ok\n'
