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

require_file "docs/architecture/38-graph-relation-write-path.md"
require_file "docs/engineering/graph-relation-write-path.md"
require_file "docs/adr/0038-graph-relation-write-path.md"
require_file "docs/archive/engineering/waves/spine41-graph-relation-write-path.md"
require_file "tests/smoke/graph-relation-write-path/test_graph_relation_write_path.sh"

for file in \
  "docs/architecture/38-graph-relation-write-path.md" \
  "docs/engineering/graph-relation-write-path.md" \
  "docs/adr/0038-graph-relation-write-path.md"; do
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
  "docs/engineering/command-surface.md" \
  "docs/engineering/testing.md" \
  "docs/engineering/current-status.md" \
  "docs/engineering/four-repo-roadmap.md" \
  "docs/engineering/data-plane-roadmap.md" \
  "docs/labs/filesystem-loop/runbook.md"; do
  require_phrase "$file" "yai.graph_relation.v1"
  require_phrase "$file" "graph materialize"
  require_phrase "$file" "graph relations"
  require_phrase "$file" "RuntimeGraph remains planned"
  require_phrase "$file" "Ladybug integration remains future"
done

for file in \
  "docs/engineering/command-surface.md" \
  "docs/engineering/data-plane-roadmap.md" \
  "docs/labs/filesystem-loop/runbook.md"; do
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
require_phrase "docs/internal/extraction-inventory.tsv" "SPINE.41"

if grep -RIn 'docs/manuals/manual-filesystem-loop-validation' \
  docs/engineering docs/architecture docs/labs README.md 2>/dev/null |
  grep -v 'not the active' |
  grep -v 'replaces' |
  grep -v 'historical' |
  grep -v 'pre-redirect' >/dev/null; then
  printf 'active docs contain unqualified old filesystem-loop manual path\n' >&2
  exit 1
fi
