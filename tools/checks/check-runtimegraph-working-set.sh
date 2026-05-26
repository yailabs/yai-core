#!/bin/sh
# YAI - RuntimeGraph working set guard
#
# Purpose:
#   Ensure SPINE.42 keeps RuntimeGraph active-minimal, in-memory and loaded
#   from graph relations without claiming resident service, HNSW or Context
#   Compiler implementation.
set -eu

require_file() {
  if [ ! -f "$1" ]; then
    printf 'missing runtimegraph working set file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing runtimegraph working set phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "docs/architecture/39-runtimegraph-working-set.md"
require_file "docs/engineering/runtimegraph-working-set.md"
require_file "docs/adr/0039-runtimegraph-working-set.md"
require_file "docs/archive/engineering/waves/spine42-runtimegraph-working-set.md"
require_file "tests/smoke/runtimegraph-working-set/test_runtimegraph_working_set.sh"

for file in \
  "docs/architecture/39-runtimegraph-working-set.md" \
  "docs/engineering/runtimegraph-working-set.md" \
  "docs/adr/0039-runtimegraph-working-set.md" \
  "docs/archive/engineering/waves/spine42-runtimegraph-working-set.md" \
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md" \
  "docs/engineering/current-status.md" \
  "docs/engineering/four-repo-roadmap.md" \
  "docs/engineering/data-plane-roadmap.md" \
  "docs/architecture/37-graph-persistence-runtimegraph.md" \
  "docs/architecture/38-graph-relation-write-path.md" \
  "docs/engineering/graph-persistence-runtimegraph.md" \
  "docs/engineering/graph-relation-write-path.md" \
  "docs/labs/filesystem-loop/runbook.md" \
  "docs/labs/filesystem-loop/tests.md" \
  "docs/labs/filesystem-loop/outputs.md"; do
  require_phrase "$file" "RuntimeGraph"
  require_phrase "$file" "in-memory working set"
done

for file in \
  "docs/architecture/39-runtimegraph-working-set.md" \
  "docs/engineering/runtimegraph-working-set.md" \
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md" \
  "docs/engineering/graph-persistence-runtimegraph.md" \
  "docs/labs/filesystem-loop/runbook.md" \
  "docs/labs/filesystem-loop/tests.md" \
  "docs/labs/filesystem-loop/outputs.md"; do
  require_phrase "$file" "per-command ephemeral"
  require_phrase "$file" "resident_service: planned"
  require_phrase "$file" "durable_truth: graph_persistence"
  require_phrase "$file" "runtime-load"
  require_phrase "$file" "runtime-status"
  require_phrase "$file" "hnsw: future_candidate_index"
  require_phrase "$file" "context_compiler: future_consumer"
done

require_phrase "cmd/yai/src/main.rs" "graph runtime-load --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "graph runtime-summary --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "per_command_ephemeral"
require_phrase "engine/yai-engine/src/store/lmdb.rs" "RuntimeGraphLoadResult"
require_phrase "engine/yai-engine/src/store/lmdb.rs" "load_runtime_graph_for_case"
require_phrase "Makefile" "check-runtimegraph-working-set"
require_phrase "Makefile" "smoke-spine42"
require_phrase "docs/internal/extraction-inventory.tsv" "SPINE.42"

if grep -RIn 'docs/manuals/manual-filesystem-loop-validation' \
  docs/engineering docs/architecture docs/labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' |
  grep -v 'pre-redirect' >/dev/null; then
  printf 'active docs contain unqualified old filesystem-loop manual path\n' >&2
  exit 1
fi

printf 'check-runtimegraph-working-set: ok\n'
