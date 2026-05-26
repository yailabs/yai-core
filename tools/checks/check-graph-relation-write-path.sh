#!/bin/sh
# YAI - graph relation write path guard
#
# Purpose:
#   Ensure SPINE.41 keeps graph relation materialization explicit without
#   claiming RuntimeGraph or Ladybug implementation.
set -eu

require_file() {
  if [ ! -f "$1" ]; then
    printf 'missing graph relation file: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  file="$1"
  phrase="$2"
  if ! grep -Fq "$phrase" "$file"; then
    printf 'missing graph relation phrase in %s: %s\n' "$file" "$phrase" >&2
    exit 1
  fi
}

require_file "work/archive/architecture-snapshots/38-graph-relation-write-path.md"
require_file "work/archive/engineering-snapshots/graph-relation-write-path.md"
require_file "work/archive/adr/0038-graph-relation-write-path.md"
require_file "work/archive/legacy-docs/engineering/waves/spine41-graph-relation-write-path.md"
require_file "tests/smoke/graph-relation-write-path/test_graph_relation_write_path.sh"

for file in \
  "work/archive/architecture-snapshots/38-graph-relation-write-path.md" \
  "work/archive/engineering-snapshots/graph-relation-write-path.md" \
  "work/archive/adr/0038-graph-relation-write-path.md"; do
  require_phrase "$file" "yai.graph_relation.v1"
  require_phrase "$file" "graph materialize"
  require_phrase "$file" "graph relations"
  require_phrase "$file" "decision_controls_attempt"
  require_phrase "$file" "receipt_records_effect"
  require_phrase "$file" "relation_id"
  require_phrase "$file" "source_record_id"
  require_phrase "$file" "RuntimeGraph remains planned"
  require_phrase "$file" "Ladybug integration remains future"
done

for file in \
  "work/spines/command-surface.md" \
  "work/spines/testing.md" \
  "work/spines/current-status.md" \
  "work/spines/yai-spine.md" \
  "work/spines/data-plane.md" \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "yai.graph_relation.v1"
  require_phrase "$file" "graph materialize"
  require_phrase "$file" "graph relations"
  require_phrase "$file" "RuntimeGraph remains planned"
  require_phrase "$file" "Ladybug integration remains future"
done

for file in \
  "work/spines/command-surface.md" \
  "work/spines/data-plane.md" \
  "labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "decision_controls_attempt"
  require_phrase "$file" "receipt_records_effect"
  require_phrase "$file" "relation_id"
  require_phrase "$file" "source_record_id"
done

require_phrase "cmd/yai/src/main.rs" "graph materialize --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "graph relations --case <case_ref>"
require_phrase "cmd/yai/src/main.rs" "GRAPH_RELATION_SCHEMA"
require_phrase "engine/yai-engine/src/store/lmdb.rs" "GRAPH_RELATION_SCHEMA"
require_phrase "engine/yai-engine/src/store/lmdb.rs" "lmdb_graph_relations_v0"
require_phrase "Makefile" "check-graph-relation-write-path"
require_phrase "Makefile" "smoke-spine41"
require_phrase "work/inventories/extraction-inventory.tsv" "SPINE.41"

if grep -RIn 'historical filesystem-loop manual' \
  docs/engineering docs/architecture docs/labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' |
  grep -v 'pre-redirect' >/dev/null; then
  printf 'active docs contain unqualified legacy filesystem-loop manual path\n' >&2
  exit 1
fi
