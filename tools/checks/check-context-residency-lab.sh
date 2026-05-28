#!/usr/bin/env bash
# YAI - context residency lab guard
#
# Purpose:
#   Ensure LAB.TEST.2S files, references and benchmark matrix entrypoints exist.
#
# Scope:
#   Validates the context-residency lab surface and its filesystem-loop links.
#
# Non-goals:
#   Does not execute provider-backed benchmark modes or validate model output.
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
cd "$ROOT"

fail() {
  printf 'check-context-residency-lab: %s\n' "$*" >&2
  exit 1
}

require_file() {
  [ -f "$1" ] || fail "missing required file: $1"
}

require_dir() {
  [ -d "$1" ] || fail "missing required directory: $1"
}

require_phrase() {
  grep -F -- "$2" "$1" >/dev/null || fail "$1 missing phrase: $2"
}

require_file "labs/context-residency/README.md"
require_file "labs/context-residency/runbook.md"
require_file "labs/context-residency/tests.md"
require_file "labs/context-residency/outputs.md"
require_file "labs/context-residency/notebook.ipynb"
require_file "labs/context-residency/prompts.json"
require_file "labs/context-residency/run.sh"
require_file "labs/context-residency/config.example.env"
require_file "labs/context-residency/bin/run-matrix.py"
require_file "labs/context-residency/bin/build-corpus.py"
require_file "labs/context-residency/bin/mini-rag.py"
require_file "labs/context-residency/bin/score-comparisons.py"
require_file "labs/context-residency/bin/render-report.py"
require_dir "labs/context-residency/runs"
require_file "labs/context-residency/fixtures/base-frame.md"
require_file "labs/context-residency/fixtures/delta-frame.md"
require_file "labs/context-residency/fixtures/task-frame.md"
require_file "labs/context-residency/fixtures/policy-scope.json"
require_file "labs/context-residency/fixtures/case-model-session.json"
require_file "labs/context-residency/fixtures/invalidation-state.json"

[ -x "labs/context-residency/run.sh" ] || fail "run.sh is not executable"
bash -n "labs/context-residency/run.sh" || fail "run.sh syntax failed"
python3 -m py_compile \
  labs/context-residency/bin/run-matrix.py \
  labs/context-residency/bin/build-corpus.py \
  labs/context-residency/bin/mini-rag.py \
  labs/context-residency/bin/score-comparisons.py \
  labs/context-residency/bin/render-report.py
python3 -m json.tool "labs/context-residency/prompts.json" >/dev/null
python3 -m json.tool "labs/context-residency/fixtures/policy-scope.json" >/dev/null
python3 -m json.tool "labs/context-residency/fixtures/case-model-session.json" >/dev/null
python3 -m json.tool "labs/context-residency/fixtures/invalidation-state.json" >/dev/null

require_phrase "labs/registry.md" "Test 2"
require_phrase "labs/registry.md" "context-residency"
require_phrase "labs/filesystem-loop/README.md" "Related benchmark"
require_phrase "labs/filesystem-loop/README.md" "labs/context-residency"
require_phrase "labs/filesystem-loop/runbook.md" "labs/context-residency"
require_phrase "labs/filesystem-loop/tests.md" "labs/context-residency"
require_phrase "labs/filesystem-loop/outputs.md" "labs/context-residency"
require_phrase "labs/context-residency/run.sh" "no-ai"
require_phrase "labs/context-residency/run.sh" "case-native"
require_phrase "labs/context-residency/run.sh" "--mode no-ai|case-native|naked|raw-context|mini-rag|compare|synthetic|all"
require_phrase "labs/context-residency/run.sh" "raw-context"
require_phrase "labs/context-residency/run.sh" "mini-rag"
require_phrase "labs/context-residency/run.sh" "Direct HTTP provider"
require_phrase "labs/context-residency/run.sh" "calls are allowed only for naked/direct baselines C0-C3"
require_phrase "labs/context-residency/run.sh" "bin/run-matrix.py"
require_phrase "labs/context-residency/bin/run-matrix.py" "provider-start-external-dry-run"
require_phrase "labs/context-residency/bin/run-matrix.py" "\"provider\""
require_phrase "labs/context-residency/bin/run-matrix.py" "\"start\""
require_phrase "labs/context-residency/bin/run-matrix.py" "attach-provider"
require_phrase "labs/context-residency/bin/run-matrix.py" "\"prompt\""
require_phrase "labs/context-residency/bin/run-matrix.py" "commands.log"
require_phrase "labs/context-residency/bin/run-matrix.py" "direct_provider("
require_phrase "labs/context-residency/bin/run-matrix.py" "\"yai_cli\""
require_phrase "labs/context-residency/bin/run-matrix.py" "A naked no-context answer is a negative control"
require_phrase "labs/context-residency/README.md" "Test 2 is a benchmark family"
require_phrase "labs/context-residency/README.md" "fixture/synthetic renderer is retained only as a non-canonical microbenchmark"
require_phrase "labs/context-residency/README.md" "Direct provider calls are allowed only for naked baselines"
require_phrase "labs/context-residency/tests.md" "naked_no_context condition is a"
require_phrase "labs/context-residency/tests.md" "no fixture/synthetic benchmark is reported as canonical"
require_phrase "labs/context-residency/outputs.md" "schema: yai.context_residency.matrix.metrics.v1"
require_phrase "labs/context-residency/outputs.md" "scorecard.json"
require_phrase "labs/context-residency/outputs.md" "transcript.md"
require_phrase "labs/context-residency/outputs.md" "assets/corpus/chunks.jsonl"
require_phrase "labs/context-residency/bin/run-matrix.py" "render_execution_markdown"
require_phrase "labs/context-residency/prompts.json" "Q10-continuity"
require_phrase "labs/context-residency/prompts.json" "multi_turn_continuity"

if grep -F "synthetic" labs/context-residency/README.md | grep -F "non-canonical" >/dev/null; then
  :
else
  fail "README must demote synthetic mode"
fi

printf 'check-context-residency-lab: ok\n'
