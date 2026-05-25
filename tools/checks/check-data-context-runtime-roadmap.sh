#!/usr/bin/env bash
set -euo pipefail

# YAI - Data Context Runtime roadmap guard
#
# Purpose:
#   Guard SPINE.33M durable truth / RuntimeGraph / HNSW / Context Compiler
#   doctrine.
#
# Boundary:
#   This script validates docs only. It does not implement RuntimeGraph,
#   HNSW, graph persistence or context compilation.

require_file() {
  test -f "$1" || {
    printf 'missing required file: %s\n' "$1" >&2
    exit 1
  }
}

require_text() {
  local file="$1"
  local text="$2"
  grep -F -- "$text" "$file" >/dev/null || {
    printf 'missing required text in %s: %s\n' "$file" "$text" >&2
    exit 1
  }
}

require_file docs/architecture/31-data-context-runtime-runtimegraph.md
require_file docs/engineering/data-context-runtime-runtimegraph.md
require_file docs/adr/0031-data-context-runtime-runtimegraph.md
require_file docs/archive/engineering/waves/spine33m-data-context-runtime-runtimegraph.md

for text in \
  "Truth lives on durable planes" \
  "Computation happens in runtime working sets" \
  "LMDB stores records" \
  "Ladybug stores relations" \
  "DuckDB stores facts" \
  "RuntimeGraph computes over the active case" \
  "HNSW finds candidate nodes" \
  "Context Compiler renders controlled views" \
  "HNSW finds vectors" \
  "RuntimeGraph gives meaning" \
  "vector_id -> retrieval_unit_id -> runtime_node_id" \
  "ContextFrame is a temporary compiled artifact" \
  "SPINE.40 Graph Persistence / RuntimeGraph Doctrine + Schema" \
  "SPINE.42 RuntimeGraph In-Memory Working Set" \
  "SPINE.58D HNSW Candidate -> RuntimeGraph Expansion"
do
  require_text docs/engineering/data-context-runtime-runtimegraph.md "$text"
done

for file in docs/engineering/four-repo-roadmap.md docs/engineering/data-plane-roadmap.md docs/architecture/00-spine.md; do
  require_text "$file" "RuntimeGraph"
  require_text "$file" "Data Context Runtime"
  require_text "$file" "HNSW Candidate -> RuntimeGraph Expansion"
done

printf "data-context-runtime-roadmap: ok\n"
