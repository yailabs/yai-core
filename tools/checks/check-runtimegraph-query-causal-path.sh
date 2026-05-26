#!/usr/bin/env bash
set -euo pipefail

# YAI - RuntimeGraph query / causal path guard
#
# Purpose:
#   Ensure SPINE.44 stays bounded, diagnostic and RuntimeGraph-backed.

missing_file() {
  printf 'missing runtimegraph query file: %s\n' "$1" >&2
  exit 1
}

require_file() {
  test -f "$1" || missing_file "$1"
}

require_phrase() {
  local file="$1"
  local phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing runtimegraph query phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/40-runtimegraph-query-causal-path.md"
require_file "work/archive/engineering-snapshots/runtimegraph-query-causal-path.md"
require_file "work/archive/adr/0040-runtimegraph-query-causal-path.md"
require_file "work/archive/legacy-docs/engineering/waves/spine44-runtimegraph-query-causal-path.md"
require_file "labs/filesystem-loop/runbook.md"
require_file "tests/smoke/runtimegraph-query/test_runtimegraph_query.sh"

for file in \
  "work/archive/architecture-snapshots/40-runtimegraph-query-causal-path.md" \
  "work/archive/engineering-snapshots/runtimegraph-query-causal-path.md" \
  "work/archive/adr/0040-runtimegraph-query-causal-path.md" \
  "work/archive/legacy-docs/engineering/waves/spine44-runtimegraph-query-causal-path.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "fanout"
  require_phrase "$file" "fanin"
  require_phrase "$file" "neighborhood"
  require_phrase "$file" "bounded traversal"
  require_phrase "$file" "causal path"
  require_phrase "$file" "max-depth"
  require_phrase "$file" "edge-kind filter"
  require_phrase "$file" "RuntimeGraph is not a generic graph database"
  require_phrase "$file" "color-aware graph inspection"
  require_phrase "$file" "plain output remains parseable"
done

require_phrase "work/archive/architecture-snapshots/40-runtimegraph-query-causal-path.md" "labs/filesystem-loop"
require_phrase "cmd/yai/src/main.rs" "graph fanout --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "graph_path:"
require_phrase "Makefile" "smoke-spine44"
