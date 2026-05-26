#!/bin/sh
# YAI - graph persistence / RuntimeGraph doctrine guard
#
# Purpose:
#   Ensure SPINE.40 keeps durable graph truth, RuntimeGraph computation, HNSW
#   candidate retrieval and Context Compiler view rendering separate.
set -eu

require_file() {
  if [ ! -f "$1" ]; then
    printf 'missing graph/runtimegraph file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing graph/runtimegraph phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/37-graph-persistence-runtimegraph.md"
require_file "work/archive/engineering-snapshots/graph-persistence-runtimegraph.md"
require_file "work/archive/adr/0037-graph-persistence-runtimegraph.md"
require_file "work/archive/legacy-docs/engineering/waves/spine40-graph-persistence-runtimegraph.md"
require_file "include/yai/graph/node_kind.h"
require_file "include/yai/graph/edge_kind.h"
require_file "include/yai/graph/runtime_graph.h"
require_file "system/graph/node_kind.c"
require_file "system/graph/edge_kind.c"
require_file "system/graph/runtime_graph.c"
require_file "tests/smoke/graph-runtimegraph-schema/test_graph_runtimegraph_schema.c"

for file in \
  "work/archive/architecture-snapshots/37-graph-persistence-runtimegraph.md" \
  "work/archive/engineering-snapshots/graph-persistence-runtimegraph.md" \
  "work/archive/adr/0037-graph-persistence-runtimegraph.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "work/spines/current-status.md" \
  "work/spines/yai-spine.md" \
  "work/spines/data-plane.md" \
  "work/archive/architecture-snapshots/00-spine.md" \
  "work/archive/architecture-snapshots/26-context-compiler-retrieval-boundary.md"; do
  require_phrase "$file" "Graph persistence"
  require_phrase "$file" "RuntimeGraph"
  require_phrase "$file" "durable typed relations"
  require_phrase "$file" "in-memory active case working set"
  require_phrase "$file" "HNSW finds candidate nodes"
  require_phrase "$file" "HNSW is not graph truth"
  require_phrase "$file" "Context Compiler"
  require_phrase "$file" "Projection does not disappear"
  require_phrase "$file" "Persistent truth on disk"
  require_phrase "$file" "Computational shape in memory"
done

require_phrase "cmd/yai/src/main.rs" "yai graph schema"
require_phrase "cmd/yai/src/main.rs" "yai graph runtime-status"
require_phrase "Makefile" "check-graph-runtimegraph-doctrine"
require_phrase "Makefile" "smoke-spine40"
require_phrase "work/inventories/extraction-inventory.tsv" "SPINE.40"

if grep -RIn 'historical filesystem-loop manual' \
  work/spines docs/architecture labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' |
  grep -v 'pre-redirect' >/dev/null; then
  printf 'active docs contain unqualified legacy filesystem-loop manual path\n' >&2
  exit 1
fi
