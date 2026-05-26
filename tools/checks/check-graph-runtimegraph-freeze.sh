#!/usr/bin/env bash
set -euo pipefail

# YAI - Graph + RuntimeGraph freeze guard
#
# Purpose:
#   Keep SPINE.45 focused on the frozen graph/runtimegraph lifecycle and the
#   review/control visibility introduced in SPINE.44A-C.

require_file() {
  if [[ ! -f "$1" ]]; then
    printf 'missing graph runtimegraph freeze file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  local corpus="$1"
  local phrase="$2"
  if [[ "$corpus" != *"$phrase"* ]]; then
    printf 'missing graph runtimegraph freeze phrase: %s\n' "$phrase" >&2
    exit 1
  fi
}

files=(
  "work/archive/architecture-snapshots/44-graph-runtimegraph-freeze.md"
  "work/archive/engineering-snapshots/graph-runtimegraph-freeze.md"
  "work/archive/adr/0044-graph-runtimegraph-freeze.md"
  "work/archive/legacy-docs/engineering/waves/spine45-graph-runtimegraph-freeze.md"
  "work/spines/command-surface.md"
  "work/spines/testing.md"
  "work/spines/current-status.md"
  "work/spines/yai-spine.md"
  "work/spines/data-plane.md"
  "work/archive/architecture-snapshots/37-graph-persistence-runtimegraph.md"
  "work/archive/architecture-snapshots/38-graph-relation-write-path.md"
  "work/archive/architecture-snapshots/39-runtimegraph-working-set.md"
  "work/archive/architecture-snapshots/40-runtimegraph-rebuild.md"
  "work/archive/architecture-snapshots/40-runtimegraph-query-causal-path.md"
  "work/archive/architecture-snapshots/42-operator-review-deferred-loop.md"
  "work/archive/architecture-snapshots/43-cli-review-interaction-surface.md"
  "work/archive/engineering-snapshots/graph-persistence-runtimegraph.md"
  "work/archive/engineering-snapshots/graph-relation-write-path.md"
  "work/archive/engineering-snapshots/runtimegraph-working-set.md"
  "work/archive/engineering-snapshots/runtimegraph-rebuild.md"
  "work/archive/engineering-snapshots/runtimegraph-query-causal-path.md"
  "work/archive/engineering-snapshots/operator-review-deferred-loop.md"
  "work/archive/engineering-snapshots/cli-review-interaction-surface.md"
  "labs/filesystem-loop/README.md"
  "labs/filesystem-loop/runbook.md"
)

for file in "${files[@]}"; do
  require_file "$file"
done

corpus="$(cat "${files[@]}" | tr '\n' ' ')"

for phrase in \
  "graph schema" \
  "yai.graph_relation.v1" \
  "graph materialize" \
  "graph relations" \
  "RuntimeGraph" \
  "runtime-load" \
  "runtime-summary" \
  "runtime graph rebuild" \
  "yai.runtime_graph_rebuild_report.v1" \
  "fanout" \
  "fanin" \
  "neighborhood" \
  "causal path" \
  "bounded traversal" \
  "edge-kind filter" \
  "plain output remains parseable" \
  "color-aware graph inspection" \
  "RuntimeGraph is not durable truth" \
  "HNSW future" \
  "Context Compiler future" \
  "Ladybug future persistence integration" \
  "review_request" \
  "review_decision" \
  "control_pending" \
  "approve" \
  "deny" \
  "defer" \
  "quarantine"; do
  require_phrase "$corpus" "$phrase"
done

require_phrase "$(cat Makefile)" "check-graph-runtimegraph-freeze"
require_phrase "$(cat Makefile)" "smoke-spine45"
require_phrase "$(cat tests/smoke/graph-runtimegraph-freeze/test_graph_runtimegraph_freeze.sh)" "graph_freeze:review_records visible_or_classified"

printf 'check-graph-runtimegraph-freeze: ok\n'
