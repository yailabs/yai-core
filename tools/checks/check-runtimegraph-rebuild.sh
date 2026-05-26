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

require_file "docs/architecture/40-runtimegraph-rebuild.md"
require_file "docs/engineering/runtimegraph-rebuild.md"
require_file "docs/adr/0040-runtimegraph-rebuild.md"
require_file "docs/archive/engineering/waves/spine43-runtimegraph-rebuild.md"
require_file "tests/smoke/runtimegraph-rebuild/test_runtimegraph_rebuild.sh"

for file in \
  "docs/architecture/40-runtimegraph-rebuild.md" \
  "docs/engineering/runtimegraph-rebuild.md" \
  "docs/adr/0040-runtimegraph-rebuild.md" \
  "docs/archive/engineering/waves/spine43-runtimegraph-rebuild.md" \
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md" \
  "docs/engineering/current-status.md" \
  "docs/engineering/four-repo-roadmap.md" \
  "docs/engineering/data-plane-roadmap.md" \
  "docs/architecture/39-runtimegraph-working-set.md" \
  "docs/engineering/runtimegraph-working-set.md" \
  "docs/labs/filesystem-loop/runbook.md" \
  "docs/labs/filesystem-loop/tests.md" \
  "docs/labs/filesystem-loop/outputs.md"; do
  require_phrase "$file" "RuntimeGraph rebuild"
  require_phrase "$file" "yai.runtime_graph_rebuild_report.v1"
  require_phrase "$file" "journal"
  require_phrase "$file" "LMDB"
  require_phrase "$file" "graph relations"
  require_phrase "$file" "runtime-summary"
  require_phrase "$file" "runtime_graph_rebuild"
  require_phrase "$file" "resident service planned"
  require_phrase "$file" "RuntimeGraph is not durable truth"
  require_phrase "$file" "docs/labs/filesystem-loop"
done

require_phrase "cmd/yai/src/main.rs" "graph rebuild --case <case_ref> --from journal"
require_phrase "cmd/yai/src/main.rs" "graph rebuild-report --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "yai.runtime_graph_rebuild_report.v1"
require_phrase "Makefile" "check-runtimegraph-rebuild"
require_phrase "Makefile" "smoke-spine43"
require_phrase "docs/internal/extraction-inventory.tsv" "SPINE.43"

if grep -RIn 'docs/manuals/manual-filesystem-loop-validation' \
  docs/engineering docs/architecture docs/labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' |
  grep -v 'pre-redirect' >/dev/null; then
  printf 'active docs contain unqualified old filesystem-loop manual path\n' >&2
  exit 1
fi

printf 'check-runtimegraph-rebuild: ok\n'
