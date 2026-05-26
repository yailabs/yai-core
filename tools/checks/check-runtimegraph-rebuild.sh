#!/bin/sh
# YAI - RuntimeGraph rebuild guard
#
# Purpose:
#   Ensure SPINE.43 keeps RuntimeGraph rebuild diagnostic, replay-backed and
#   non-durable.
set -eu

require_file() {
  if [ ! -f "$1" ]; then
    printf 'missing runtimegraph rebuild file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing runtimegraph rebuild phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/40-runtimegraph-rebuild.md"
require_file "work/archive/engineering-snapshots/runtimegraph-rebuild.md"
require_file "work/archive/adr/0040-runtimegraph-rebuild.md"
require_file "work/archive/legacy-docs/engineering/waves/spine43-runtimegraph-rebuild.md"
require_file "tests/smoke/runtimegraph-rebuild/test_runtimegraph_rebuild.sh"

for file in \
  "work/archive/architecture-snapshots/40-runtimegraph-rebuild.md" \
  "work/archive/engineering-snapshots/runtimegraph-rebuild.md" \
  "work/archive/adr/0040-runtimegraph-rebuild.md" \
  "work/archive/legacy-docs/engineering/waves/spine43-runtimegraph-rebuild.md" \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "work/spines/current-status.md" \
  "work/spines/yai-spine.md" \
  "work/spines/data-plane.md" \
  "work/archive/architecture-snapshots/39-runtimegraph-working-set.md" \
  "work/archive/engineering-snapshots/runtimegraph-working-set.md" \
  "labs/filesystem-loop/README.md" \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "RuntimeGraph rebuild"
  require_phrase "$file" "yai.runtime_graph_rebuild_report.v1"
  require_phrase "$file" "journal"
  require_phrase "$file" "LMDB"
  require_phrase "$file" "graph relations"
  require_phrase "$file" "runtime-summary"
  require_phrase "$file" "runtime_graph_rebuild"
  require_phrase "$file" "resident service planned"
  require_phrase "$file" "RuntimeGraph is not durable truth"
  require_phrase "$file" "labs/filesystem-loop"
done

require_phrase "cmd/yai/src/main.rs" "graph rebuild --case <case_ref> --from journal"
require_phrase "cmd/yai/src/main.rs" "graph rebuild-report --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "yai.runtime_graph_rebuild_report.v1"
require_phrase "Makefile" "check-runtimegraph-rebuild"
require_phrase "Makefile" "smoke-spine43"
require_phrase "work/inventories/extraction-inventory.tsv" "SPINE.43"

if grep -RIn 'historical filesystem-loop manual' \
  work/spines docs/architecture labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' |
  grep -v 'pre-redirect' >/dev/null; then
  printf 'active docs contain unqualified legacy filesystem-loop manual path\n' >&2
  exit 1
fi

printf 'check-runtimegraph-rebuild: ok\n'
