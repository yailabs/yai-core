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
  "docs/architecture/44-graph-runtimegraph-freeze.md"
  "docs/engineering/graph-runtimegraph-freeze.md"
  "docs/adr/0044-graph-runtimegraph-freeze.md"
  "docs/archive/engineering/waves/spine45-graph-runtimegraph-freeze.md"
  "docs/engineering/command-surface.md"
  "docs/engineering/testing.md"
  "docs/engineering/current-status.md"
  "docs/engineering/four-repo-roadmap.md"
  "docs/engineering/data-plane-roadmap.md"
  "docs/architecture/37-graph-persistence-runtimegraph.md"
  "docs/architecture/38-graph-relation-write-path.md"
  "docs/architecture/39-runtimegraph-working-set.md"
  "docs/architecture/40-runtimegraph-rebuild.md"
  "docs/architecture/40-runtimegraph-query-causal-path.md"
  "docs/architecture/42-operator-review-deferred-loop.md"
  "docs/architecture/43-cli-review-interaction-surface.md"
  "docs/engineering/graph-persistence-runtimegraph.md"
  "docs/engineering/graph-relation-write-path.md"
  "docs/engineering/runtimegraph-working-set.md"
  "docs/engineering/runtimegraph-rebuild.md"
  "docs/engineering/runtimegraph-query-causal-path.md"
  "docs/engineering/operator-review-deferred-loop.md"
  "docs/engineering/cli-review-interaction-surface.md"
  "docs/labs/filesystem-loop/README.md"
  "docs/labs/filesystem-loop/runbook.md"
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
