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

require_file "work/archive/architecture-snapshots/39-runtimegraph-working-set.md"
require_file "work/archive/engineering-snapshots/runtimegraph-working-set.md"
require_file "work/archive/adr/0039-runtimegraph-working-set.md"
require_file "work/archive/legacy-docs/engineering/waves/spine42-runtimegraph-working-set.md"
require_file "tests/smoke/runtimegraph-working-set/test_runtimegraph_working_set.sh"

for file in \
  "work/archive/architecture-snapshots/39-runtimegraph-working-set.md" \
  "work/archive/engineering-snapshots/runtimegraph-working-set.md" \
  "work/archive/adr/0039-runtimegraph-working-set.md" \
  "work/archive/legacy-docs/engineering/waves/spine42-runtimegraph-working-set.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "work/spines/current-status.md" \
  "work/spines/yai-spine.md" \
  "work/spines/data-plane.md" \
  "work/archive/architecture-snapshots/37-graph-persistence-runtimegraph.md" \
  "work/archive/architecture-snapshots/38-graph-relation-write-path.md" \
  "work/archive/engineering-snapshots/graph-persistence-runtimegraph.md" \
  "work/archive/engineering-snapshots/graph-relation-write-path.md" \
  "labs/filesystem-loop/README.md" \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "RuntimeGraph"
  require_phrase "$file" "in-memory working set"
done

for file in \
  "work/archive/architecture-snapshots/39-runtimegraph-working-set.md" \
  "work/archive/engineering-snapshots/runtimegraph-working-set.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "work/archive/engineering-snapshots/graph-persistence-runtimegraph.md" \
  "labs/filesystem-loop/README.md" \
  "labs/filesystem-loop/runbook.md"; do
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
require_phrase "work/inventories/extraction-inventory.tsv" "SPINE.42"

if grep -RIn 'historical filesystem-loop manual' \
  docs/engineering docs/architecture docs/labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' |
  grep -v 'pre-redirect' >/dev/null; then
  printf 'active docs contain unqualified legacy filesystem-loop manual path\n' >&2
  exit 1
fi

printf 'check-runtimegraph-working-set: ok\n'
